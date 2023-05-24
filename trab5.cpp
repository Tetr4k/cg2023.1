#include "Render2D_v1.h"
#include "bezier.h"
#include "matrix.h"
#include "polygon_triangulation.h"
#include <Math.h>

int main(){
	std::vector<vec2> CP = loadCurve("borboleta.txt");
	std::vector<vec2> P = sample_bezier_spline<3>(CP, 30);
	std::vector<unsigned int> indexes = triangulate_polygon(P);

	vec2 v = {350, 350};

	mat3 Tr = {
		1.0, 0.0, v[0],
		0.0, 1.0, v[1],
		0.0, 0.0, 1.0
	};

	mat3 Ti = {
		1.0, 0.0, -v[0],
		0.0, 1.0, -v[1],
		0.0, 0.0, 1.0
	};

	LineStrip L{P.size()};
	Elements<TriangleStrip> T{indexes};

	ImageRGB Img(700, 700);
	Img.fill(white);

	for(int t=0; t<12; t++){

		float r = 2*3.14/12*t;

		mat3 R = {
			cos(r),-sin(r),	0.0,
			sin(r),	cos(r),	0.0,
			0.0,	0.0,	1.0
		};

		std::vector<vec2> Q = (Tr*R*Ti)*P;

		RGB color = lerp((float)t/11, red, yellow);

		render2d(Q, T, color, Img);
		render2d(Q, L, black, Img);
	}

	Img.savePNG("output.png");
}
