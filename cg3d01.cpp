#include "Render3D.h"

struct Shader{
    using Varying = vec2;

    void vertexShader(vec2 v, Varying& out){
	    out = v;
	};

    void fragmentShader(Varying, RGB& fragColor){
        fragColor = blue;
    }
};

int main(){
    Shader shader;

	vec2 V[] = {
		{-0.8, -0.6}, {-0.6,  0.8}, {-0.2, -0.6},
		{ 0.0,  0.0}, {0.6,  0.8},	{ 0.8, -0.8}
	};

    Triangles T{6};

    ImageRGB G{200, 50};
    G.fill(white);
    render(V, T, shader, G);
	G.savePNG("output.png");
}
