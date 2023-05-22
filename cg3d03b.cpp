#include "Render3D.h"
#include "ColorShader.h"
#include "VertexUtils.h"
#include "transforms.h"

int main(){
	Vec3Col V[] = {
		{{0, 0, 0}, red},
		{{1, 0, 0}, red},
		{{1, 1, 0}, green},
		{{0, 1, 0}, green},
		{{0, 0, 1}, blue},
		{{1, 0, 1}, blue},
		{{1, 1, 1}, yellow},
		{{0, 1, 1}, yellow}
	};

	unsigned int indices[] = {
		4, 5, 7,   6, 7, 5,
		5, 1, 6,   2, 6, 1,
		6, 2, 7,   3, 7, 2
	};

	Elements<Triangles> T{indices};

	int w = 500, h = 500;

	mat4 View = translate(0, 0, -3);
	float a = w/(float)h;
	mat4 Projection = perspective(45, a, 0.1, 10);

	ImageRGB G{w, h};

	int nframes = 60;
	for(int k = 0; k < nframes; k++){
		G.fill(white);
			
		float theta = k*2*M_PI/nframes;
	
		mat4 Model = rotate_y(theta)*rotate_x(0.1)*
		              translate(-0.5, -0.5, -0.5);
	
		ColorShader shader;
		shader.M = Projection*View*Model;
		render(V, T, shader, G);
	
		char filename[50];
		sprintf(filename, "anim/output%03d.png", k);
		puts(filename);
		G.savePNG(filename);
	}
}
