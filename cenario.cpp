#include "Image.h"

void over(RGB& Bg, RGBA C){
	vec4 vc = toVec(C);
	float alpha = vc[3];
	vec3 col = lerp(alpha, toVec(Bg), toVec3(vc));
	Bg = toColor(col);
}

void renderSprite(ImageRGBA sprite, ImageRGB& img, int x=0, int y=0, bool facingRight=true){
	if (facingRight)
		for(int j = 0; j < sprite.height(); j++)
			for(int i = 0; i < sprite.width(); i++)
				over( img(x+i, y+j), sprite(i, j) );
	else
		for(int j = 0; j < sprite.height(); j++)
			for(int i = 0; i < sprite.width(); i++)
				over( img(x-i+sprite.width(), y+j), sprite(i, j) );
}

int main(){
	ImageRGB img{"./sprites/BG.png"};

	ImageRGBA spriteWater{"./sprites/water.png"};
	for(int k=-1; k<4; k++)
		renderSprite(spriteWater, img, img.width()/2+k*spriteWater.width());

	ImageRGBA spriteFloor{"./sprites/chao.png"};
	for(int k = 0; k < img.width()/2-spriteWater.width(); k+=spriteFloor.width())
		renderSprite(spriteFloor, img, k);

	for(int k = img.width()-spriteFloor.width(); k >= img.width()/2+3*spriteWater.width(); k-=spriteFloor.width())
		renderSprite(spriteFloor, img, k);

	ImageRGBA spriteFemaleAdventurer{"./sprites/femaleAdventurer.png"};    
	int xFemaleAdventurer = 100,
		yFemaleAdventurer = spriteFloor.height();
	renderSprite(spriteFemaleAdventurer, img, xFemaleAdventurer, yFemaleAdventurer);

	ImageRGBA spriteAdventurer{"./sprites/adventurer.png"};
	int xAdventurer = spriteFemaleAdventurer.width()+200,
		yAdventurer = spriteFloor.height();
	renderSprite(spriteAdventurer, img, xAdventurer, yAdventurer);

	ImageRGBA spriteZombie{"./sprites/zombie.png"};
	int xZombie = img.width()-100-spriteZombie.width(),
		yZombie = spriteFloor.height();
	renderSprite(spriteZombie, img, xZombie, yZombie, false);

	img.savePNG("output.png");
}
