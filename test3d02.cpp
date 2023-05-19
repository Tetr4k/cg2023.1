#include "Render2D.h"
#include "matrix.h"
#include "transforms.h"

int main(){
	int w = 600, h = 600;
	float l = -0.7, r = 0.7;
	float b = -0.7, t = 0.7;
	float n = 2, f = 20;

	mat4 Model = rotate_y(0.1)*translate(-0.5, -0.5, -0.5);
	mat4 View = lookAt({0, 1, 2}, {0, 0, 0}, {0, 1, 0});
	mat4 Ortho = loadIdentity(); 
	mat4 Persp = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, (n+f)/n, -f,
		0, 0, 1/n, 0
	};
	mat4 Projection = Persp*scale(1,1,-1); // Ou Ortho
	mat4 Volume = {
		2/(r-l),      0,     0,      (l+r)/(l-r),
			0,  2/(t-b),     0,      (b+t)/(b-t),
			0,        0, 2/(f-n),    (n+f)/(n-f),
			0,        0,      0,               1
	};	
	mat4 Screen = {
		w/2.0f,      0,   0, (w-1)/2.0f,
		     0, h/2.0f,   0, (h-1)/2.0f,
		     0,      0,   1,          0,
		     0,      0,   0,          1
	};
	mat4 M = Screen*Volume*Projection*View*Model;

	std::vector<vec3> V = {
		{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
		{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1},
	};

	std::vector<vec2> P(V.size());
	for(unsigned int i = 0; i < V.size(); i++){
		vec4 v = M*toVec4(V[i], 1);
		P[i] = {v[0]/v[3], v[1]/v[3]};
	}
	
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
