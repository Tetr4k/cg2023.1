#pragma once

#include <vector>
#include "geometry.h"
#include "Image.h"
#include "Primitives.h"
#include "rasterization.h"
#include "Clip3D.h"

template<class Shader, class ImageType>
struct RenderPipeline{
	using Varying = typename Shader::Varying;

	Shader& shader;
	ImageType& image;
	
	template<class VertexAttrib, class Prims>
	void run(const VertexAttrib& V, const Prims& p){
		for(auto primitive: clip(assemble(p, transform(V)))) 
			draw(primitive);
	}

	template<class VertexAttrib>
	std::vector<Varying> transform(const VertexAttrib& V){
		std::vector<Varying> PV(std::size(V)); // requires c++17!
		for(unsigned int i = 0; i < std::size(V); i++)
			shader.vertexShader(V[i], PV[i]);
		return PV;
	}

	void draw(Line<Varying> line){
		vec4 P[] = { getPosition(line[0]), getPosition(line[1]) };
		vec2 L[] = { toScreen(P[0]), toScreen(P[1]) };
			
		for(Pixel p: rasterizeLine(L)){
			float t = find_mix_param(toVec2(p), L[0], L[1]);
			Varying vi = (1-t)*line[0] + t*line[1];
			paint(p, vi);
		}
	}
	
	void draw(Triangle<Varying> tri){
		vec4 P[] = { getPosition(tri[0]), getPosition(tri[1]), getPosition(tri[2]) };
		vec2 T[] = { toScreen(P[0]), toScreen(P[1]), toScreen(P[2]) };

		for(Pixel p: rasterizeTriangle(T)){
			vec3 t = barycentric_coords(toVec2(p), T);

			Varying vi = t[0]*tri[0] + t[1]*tri[1] + t[2]*tri[2];
			paint(p, vi);
		}
	}

	vec2 toScreen(vec4 P){
		float x = P[0];
		float y = P[1];
		float w = P[3];
		return {
			((x/w + 1)*image.width() - 1)/2,
			((y/w + 1)*image.height() - 1)/2
		};
	}

	void paint(Pixel p, Varying v){
		if(testPixel(p, v, image))
			shader.fragmentShader(v, image(p.x,p.y));
	}
};

template<class Varying>
bool testPixel(Pixel p, Varying, ImageRGB& img){
	return p.x >= 0 && p.y >= 0 && p.x < img.width() && p.y < img.height();
}

template<class VertexAttrib, class Prims, class Shader, class ImageType>
void render(const VertexAttrib& V, const Prims& p, Shader& shader, ImageType& image){
	RenderPipeline<Shader, ImageType> pipeline{shader, image};
	pipeline.run(V, p); 
}
