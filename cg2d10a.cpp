#include "Render2D_v1.h"
#include "bezier.h"
#include "polygon_triangulation.h"

int main(){
	std::vector<vec2> P = loadCurve(
		"borboleta.txt"
		//"curvaC.txt"
	);
	std::vector<vec2> Q = sample_bezier_spline<3>(P, 10);
	std::vector<unsigned int> indices = triangulate_polygon(Q);

	Elements<Triangles> T{indices};
	TriLines<Elements<Triangles>> L{indices};
	
	ImageRGB G(400, 400);
	G.fill(white);
	render2d(Q, T, red, G);
	render2d(Q, L, black, G);
	G.savePNG("output.png");
}
