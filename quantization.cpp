#include "Image.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>

int find_index(RGB color, const std::vector<RGB>& pallete){
	vec3 vc = toVec(color);
	float mind = norm2(toVec(pallete[0]) - vc);
	int mini = 0;
	for(int i = 1; i < pallete.size(); i++){
		float d = norm2(toVec(pallete[i]) - vc);
		if(d < mind){
			mind = d;
			mini = i;
		}
	}

	return mini;
}

std::vector<RGB> make_pallete(const ImageRGB& img, int K, int n_iter){
	srand(time(NULL));

	std::vector<RGB> pallete(K);
	for(RGB& color : pallete)
		color = {(Byte)rand(), (Byte)rand(), (Byte)rand()};

	for(int it = 0; it < n_iter; it++){
		std::vector<vec3> group(K);
		std::vector<int>  count(K);

		const RGB* c = img.data();
		for(int i = 0; i < img.size(); i++){
			int index = find_index(c[i], pallete);
			group[index] = group[index] + toVec(c[i]);
			count[index]++;
		}
			
		for(int i = 0; i < K; i++)
			if(count[i] != 0)
				pallete[i] = toColor(1.0/count[i]*group[i]);
	}

	return pallete;
}

int main(){
	ImageRGB img{"flowers.jpg"};
	ImageRGB res{img.width(), img.height()};

	//std::vector<RGB> pallete = {
	//	black, white, red, green, blue, cyan, yellow, magenta, gray, orange, purple, brown
	//};
	std::vector<RGB> pallete = make_pallete(img, 128, 20);

	for(int j = 0; j < img.height(); j++){
		for(int i = 0; i < img.width(); i++){
			int index = find_index(img(i, j), pallete);
			res(i,j) = pallete[index];
		}
	}

	res.savePNG("output.png");
}
