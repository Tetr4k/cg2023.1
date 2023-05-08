#ifndef CLIP2D_H
#define CLIP2D_H

#include "Primitives.h"

struct Semiplane{
	vec2 A;
	vec2 n;
	bool has(vec2 P) const{
		return dot(P-A, n) >= 0;
	}
	float intersect(vec2 P, vec2 Q) const{
		return dot(A-P, n)/dot(Q-P,n);
	}
};

struct ClipRectangle{
	float x0, y0, x1, y1;

	std::array<Semiplane, 4> sides() const{
		return {
			Semiplane{ {x0, y0}, { 1,  0} }, // left
			Semiplane{ {x1, y0}, {-1,  0} }, // right
			Semiplane{ {x0, y0}, { 0,  1} }, // down
			Semiplane{ {x1, y1}, { 0, -1} }, // up
		};
	}
};


template<class Vertex>
bool clip(Line<Vertex>& line, ClipRectangle rect){
	/**************** TAREFA - AULA 09 **************/
	return true;
}

template<class Vertex>
std::vector<Line<Vertex>> clip(const std::vector<Line<Vertex>>& lines, ClipRectangle rect){
	std::vector<Line<Vertex>> res;
	for(Line<Vertex> line: lines)
		if(clip(line, rect))
			res.push_back(line);
	return res;
}

template<class Vertex>
std::vector<Vertex> clip(const std::vector<Vertex>& polygon, Semiplane S){
	std::vector<Vertex> R;

	for(unsigned int i = 0; i < polygon.size(); i++){
		Vertex P = polygon[i];
		Vertex Q = polygon[(i+1)%polygon.size()];

		vec2 p = get2DPosition(P);
		vec2 q = get2DPosition(Q);
	
		bool Pin = S.has(p);
		bool Qin = S.has(q);

		if(Pin != Qin)
			R.push_back(lerp(S.intersect(p, q), P, Q));
		
		if(Qin)
			R.push_back(Q);
	}
	return R;
}

template<class Vertex>
std::vector<Vertex> clip(const std::vector<Vertex>& polygon, ClipRectangle rect){
	std::vector<Vertex> R = polygon;

	for(Semiplane S: rect.sides())
		R = clip(R, S);

	return R;
}

template<class Vertex>
std::vector<Triangle<Vertex>> clip(const std::vector<Triangle<Vertex>>& tris, ClipRectangle R){
	std::vector<Triangle<Vertex>> res;

	for(Triangle<Vertex> tri: tris){
		std::vector<Vertex> polygon = {tri[0], tri[1], tri[2]};
		polygon = clip(polygon, R);
		TriangleFan T{polygon.size()};
		for(Triangle<Vertex> tri: assemble(T, polygon))
			res.push_back(tri);
	}

	return res;
}

#endif
