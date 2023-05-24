#include "Render2D.h"
#include "matrix.h"
#include "transforms.h"

int main(){
	int w = 600, h = 600;
	float l = -0.7, r = 0.7;
	float b = -0.7, t = 0.7;
	float n = 1;

	mat4 Model = rotate_y(0.1)*translate(-0.5, -0.5, -0.5);
	mat4 View = lookAt({0, 1, 2}, {0, 0, 0}, {0, 1, 0});
	mat3x4 Ortho = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 0, 1
	};
	mat3x4 Persp = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1/n, 0
	};
	//mat3x4 Projection = Ortho;
	mat3x4 Projection = Persp*scale(1, 1, -1);
	mat3 Window = {
		2/(r-l),      0,       (l+r)/(l-r),
			0,  2/(t-b),       (b+t)/(b-t),
			0,        0,                1
	};	
	mat3 Screen = {
		w/2.0f, 0, (w-1)/2.0f,
		0, h/2.0f, (h-1)/2.0f,
		0,   0,     1
	};
	mat3x4 M = Screen*Window*Projection*View*Model;

	std::vector<vec3> V = {
		{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
		{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1},
	};

	std::vector<vec2> P(V.size());
	for(unsigned int i = 0; i < V.size(); i++)
		P[i] = toVec2H( M*toVec4(V[i], 1) );
	
	std::vector<unsigned int> indices = {
		0, 1,	1, 2,	2, 3,	3, 0,	4, 5,	5, 6,
		6, 7,	7, 4,	0, 4,	1, 5,	2, 6,	3, 7
	};
	Elements<Lines> L{indices};
	
	ImageRGB G(w, h);
	G.fill(white);
	render2d(P, L, red, G);
	G.savePNG("output.png");
}
