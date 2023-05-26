#include "Render3D.h"
#include "ZBuffer.h"
#include "SimpleShader.h"
#include "bezier.h"
#include "transforms.h"

int main(){
	vec3 CP[] = {
		{0, 0, 0}, 	{1, 0, 0}, 
		{1, 1, 1}, 	{0, 0, 1}
	};
	LineStrip LCP{4};

	unsigned int N = 50;
	std::vector<vec3> P = sample_bezier<3>(CP, N);
	LineStrip L{N};
	
	SimpleShader shader;

	int w = 600, h = 600;
	ImageRGB G{w, h};
	mat4 View = lookAt({4,0,1}, {0, 0, 0}, {0, 0, 1});
	float a = w/(float)h;
	mat4 Projection = perspective(50, a, 0.1, 50);

	int nframes = 30;
	for(int k = 0; k < nframes; ++k){
		G.fill(white);
		ImageZBuffer I{G};
	
		float theta = k*2*M_PI/nframes;
		mat4 Model = rotate_z(theta);
		shader.M = Projection*View*Model;
		shader.C = blue;
		render(P, L, shader, I);
		
		shader.C = red;
		render(CP, LCP, shader, I);

		char filename[50];
		sprintf(filename, "anim/output%03d.png", k);
		puts(filename);
		G.savePNG(filename);
	}
}
