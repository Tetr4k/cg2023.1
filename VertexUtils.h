#ifndef VECCOL_H
#define VECCOL_H

#include "vec.h"
#include "Color.h"

inline vec4 getPosition(vec2 v){	return toVec4(v);	}
inline vec4 getPosition(vec3 v){	return toVec4(v);	}
inline vec4 getPosition(vec4 v){	return         v;	}

template <class Vec>
struct PosCol{
	Vec position;
	RGB color;

	friend PosCol lerp(float t, PosCol u, PosCol v){
		return {lerp(t, u.position, v.position), lerp(t, u.color, v.color)};
	}

	friend vec2 get2DPosition(PosCol in){ 
		return toVec2(in.position); 
	}

	friend vec3 getColor(PosCol in){
		return toVec(in.color);
	}
};

vec2 get2DPosition(vec2 v)   {    return v;    }

template<typename Vertex>
vec4 getPosition(Vertex in){
	return toVec4(in.position);
}

using Vec2Col = PosCol<vec2>;
using Vec3Col = PosCol<vec3>;
using Vec4Col = PosCol<vec4>;

#endif
