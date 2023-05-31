#ifndef SAMPLER2D_H
#define SAMPLER2D_H

#include "Image.h"

enum Filter{NEAREST, BILINEAR};
enum WrapMode{CLAMP, REPEAT, MIRRORED_REPEAT};

class Sampler2D{
	public:
	ImageRGB img;
	Filter filter;
	WrapMode wrapX, wrapY;
	RGB default_color = magenta;

	RGB sample(vec2 texCoords) const{
		if(img.width() == 0 || img.height() == 0)
			return default_color;

		float sx = texCoords[0]*img.width() - 0.5;
		float sy = texCoords[1]*img.height() - 0.5;

		if(filter == BILINEAR)
			return sampleBI(sx, sy);

		return sampleNN(sx, sy);
	}

	private:
	RGB sampleNN(float sx, float sy) const{
		int x = round(sx);
		int y = round(sy);
		
		x = limitValue(x, img.width(), wrapX);
		y = limitValue(y, img.height(), wrapY);
		return img(x, y);
	}

	RGB sampleBI(float sx, float sy) const{
		int x = floor(sx);
		int y = floor(sy);
		float u = sx - x;
		float v = sy - y;
		int x0 = limitValue(x, img.width(), wrapX);
		int y0 = limitValue(y, img.height(), wrapY);
		int x1 = limitValue(x+1, img.width(), wrapX);
		int y1 = limitValue(y+1, img.height(), wrapY);

		return bilerp(u, v,
			img(x0, y0), img(x1, y0), 
			img(x0, y1), img(x1, y1)
		);
	}
	
	int limitValue(int v, int vmax, WrapMode wrap) const{
		if(wrap == CLAMP)
			return clamp(v, 0, vmax-1);

		int n = v>0? v/vmax: (v-vmax+1)/vmax;
		int r = v - n*vmax;

		if(wrap == MIRRORED_REPEAT && n%2!=0)
			r = vmax-1 - r;
		return r;
	}

};

#endif
