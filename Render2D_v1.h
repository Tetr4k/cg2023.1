#pragma once

#include <vector>
#include "Image.h"
#include "Primitives.h"
#include "rasterization.h"

struct Render2dPipeline{
	ImageRGB& image;

	template<class Vertices, class Prims>
	void run(const Vertices& V, const Prims& P, RGB color){
		for(auto primitive: assemble(P, V))
			draw(primitive, color);
	}

	void paint(Pixel p, RGB c){
		if(p.x >= 0 && p.y >= 0 && p.x < image.width() && p.y < image.height())
			image(p.x,p.y) = c;
	}

	void draw(Line<vec2> line, RGB color){
		for(Pixel p: rasterizeLine(line))
			paint(p, color);
	}
	
	void draw(Triangle<vec2> tri, RGB color){
		for(Pixel p: rasterizeTriangle(tri))
			paint(p, color);
	}
};

template<class Vertices, class Prims>
void render2d(const Vertices& V, const Prims& P, RGB color, ImageRGB& image){
	Render2dPipeline pipeline{image};
	pipeline.run(V, P, color);
}
