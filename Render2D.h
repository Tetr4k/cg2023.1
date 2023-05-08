#pragma once

#include <vector>
#include "geometry.h"
#include "Image.h"
#include "VertexUtils.h"
#include "Primitives.h"
#include "rasterization.h"
#include "Clip2D.h"

struct Render2dPipeline{
	ImageRGB& image;

	template<class Vertices, class Prims>
	void run(const Vertices& V, const Prims& P){
		ClipRectangle R = {-0.5f, -0.5f, image.width()-0.5f, image.height()-0.5f};
		for(auto primitive: clip(assemble(P, V), R))
			draw(primitive);
	}

	void paint(Pixel p, RGB c){
		if(p.x >= 0 && p.y >= 0 && p.x < image.width() && p.y < image.height())
			image(p.x,p.y) = c;
	}

	void draw(Line<Vec2Col> line){
		vec2 L[] = { line[0].position, line[1].position };
		RGB  C[] = { line[0].color,    line[1].color    };

		for(Pixel p: rasterizeLine(L)){
			float t = find_mix_param(toVec2(p), L[0], L[1]);
			RGB color = lerp(t, C[0], C[1]);
			paint(p, color);
		};
	}

	void draw(Triangle<Vec2Col> tri){
        // TAREFA - AULA 09
        // Para cada pixel da rasterização
        // calcule as coordenadas baricêntricas
        // use essas coordenadas para interpolar as cores
        // pinte o pixel com a cor interpolada
	}
};

template<class Vertices, class Prims>
void render2d(const Vertices& V, const Prims& P, ImageRGB& image){
	Render2dPipeline pipeline{image};
	pipeline.run(V, P);
}

template<class Vertices, class Prims>
void render2d(const Vertices& V, const Prims& P, RGB color, ImageRGB& image){
	std::vector<Vec2Col> VC;
	for(vec2 v: V)
		VC.push_back({v, color});
	render2d(VC, P, image);
}