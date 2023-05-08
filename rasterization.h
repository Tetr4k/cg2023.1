#ifndef RASTERIZATION_H
#define RASTERIZATION_H

#include <algorithm>
#include <cmath>
#include "vec.h"
#include "geometry.h"

//////////////////////////////////////////////////////////////////////////////

struct Pixel{
	int x, y;
};

inline Pixel toPixel(vec2 u){
	return { (int)round(u[0]), (int)round(u[1]) };
}

inline vec2 toVec2(Pixel p){
	return {(float)p.x, (float)p.y};
}

//////////////////////////////////////////////////////////////////////////////

template<class Line>
std::vector<Pixel> rasterizeLine(const Line& P){
	//return simple(P[0], P[1]);

	return dda(P[0], P[1]);

	//return bresenham(toPixel(P[0]), toPixel(P[1]));
}

//////////////////////////////////////////////////////////////////////////////

inline std::vector<Pixel> simple(vec2 A, vec2 B){
	std::vector<Pixel> out;
	vec2 d = B - A;
	float m = d[1]/d[0];
	float b = A[1] - m*A[0];

	int x0 = (int)roundf(A[0]);
	int x1 = (int)roundf(B[0]);

	for(int x = x0; x <= x1; x++){
		int y = (int)roundf(m*x + b);
		out.push_back({x, y});
	}
	return out;
}

//////////////////////////////////////////////////////////////////////////////

inline std::vector<Pixel> dda(vec2 A, vec2 B){
	vec2 dif = B - A;
	float delta = std::max(fabs(dif[0]), fabs(dif[1]));

	vec2 d = (1/delta)*dif;
	vec2 p = A;

	std::vector<Pixel> out;
	for(int i = 0; i <= delta; i++){
		out.push_back(toPixel(p));
		p = p + d;
	}
	return out;
}

//////////////////////////////////////////////////////////////////////////////

inline std::vector<Pixel> bresenham_base(int dx, int dy){
	std::vector<Pixel> out;

	int D = 2*dy - dx; 
	int y = 0;
	for(int x = 0; x <= dx; x++){
		out.push_back({x, y});
		if(D > 0){
			y++;
			D -= 2*dx;
		}
		D += 2*dy;
	}
	return out;
}

inline std::vector<Pixel> bresenham(int dx, int dy){
	std::vector<Pixel> out;

	if(dx >= dy){
		out = bresenham_base(dx, dy);
	}else{
		out = bresenham_base(dy, dx);
		for(Pixel& p: out)
			p = {p.y, p.x};
	}
	return out;
}

inline std::vector<Pixel> bresenham(Pixel p0, Pixel p1){
	if(p0.x > p1.x)
		std::swap(p0, p1);

	std::vector<Pixel> out = bresenham(p1.x - p0.x, abs(p1.y - p0.y));

	int s = (p0.y <= p1.y)? 1: -1;

	for(Pixel& p: out)
		p = {p0.x + p.x, p0.y + s*p.y};

	return out;
}

//////////////////////////////////////////////////////////////////////////////

template<class Tri>
std::vector<Pixel> rasterizeTriangle(const Tri& P){
	//return simple_rasterize_triangle(P);
	return scanline(P);
}

template<class Tri>
std::vector<Pixel> simple_rasterize_triangle(const Tri& P){
	vec2 A = P[0];
	vec2 B = P[1];
	vec2 C = P[2];

	int xmin =  ceil(std::min({A[0], B[0], C[0]}));
	int xmax = floor(std::max({A[0], B[0], C[0]}));
	int ymin =  ceil(std::min({A[1], B[1], C[1]}));
	int ymax = floor(std::max({A[1], B[1], C[1]}));

	std::vector<Pixel> out;
	Pixel p;
	for(p.y = ymin; p.y <= ymax; p.y++)
		for(p.x = xmin; p.x <= xmax; p.x++)
			if(is_inside(toVec2(p), P))
				out.push_back(p);
	return out;
}

template<class Tri>
bool isDegenerated(const Tri& P){
	return P[0]==P[1] && P[1]==P[2] && P[2]==P[0];
}

bool isCrossing(vec2 A, vec2 B, int y){// Se os pontos cruzarem a reta
	return (B[1]>=y && A[1]<=y) || (B[1]<=y && A[1]>=y);
}

float calcX(vec2 A, vec2 B, int y){
	return (((B[0]-A[0])*(y-A[1]))/(B[1]-A[1])) + A[0];
}

vec2 intersecs(vec2 A, vec2 B, vec2 C, int y){
	std::vector<float> xs;

	if(isCrossing(A, B, y)){
		float x = calcX(A, B, y);
		xs.push_back(x);
	}
	if(isCrossing(B, C, y)){
		float x = calcX(B, C, y);
		xs.push_back(x);
	}
	if(isCrossing(C, A, y)){
		float x = calcX(C, A, y);
		xs.push_back(x);
	}

	float xmin =  ceil(*std::min_element(xs.begin(), xs.end())),
		  xmax = floor(*std::max_element(xs.begin(), xs.end()));

	return {xmin, xmax};
}

template<class Tri>
std::vector<Pixel> scanline(const Tri& P){
	if (isDegenerated(P))
		return {};

	vec2 A = P[0];
	vec2 B = P[1];
	vec2 C = P[2];
	int ymin = 	ceil(std::min({A[1], B[1], C[1]})),
		ymax = floor(std::max({A[1], B[1], C[1]}));

	std::vector<Pixel> out;
	for(int y = ymin; y <= ymax; y++){

		vec2 xs = intersecs(A, B, C, y);

		for(int x = xs[0]; x <= xs[1]; x++)
			out.push_back({x, y});
	}
	
	return out;
}

#endif
