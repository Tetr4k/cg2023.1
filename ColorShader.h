#ifndef COLOR_SHADER_H
#define COLOR_SHADER_H

struct ColorShader{
	struct Varying{
		vec4 position;
		vec3 color;
	};
	friend Varying operator*(float t, Varying V){
		return {t*V.position, t*V.color};
	}
	friend Varying operator+(Varying U, Varying V){
		return {U.position + V.position, U.color + V.color};
	}

	mat4 M;

	template<class Vertex>
	void vertexShader(Vertex in, Varying& out){
		out.position = M*getPosition(in);
		out.color = getColor(in);
	}

	void fragmentShader(Varying V, RGB& FragColor){
		FragColor = toColor(V.color);
	}
};

#endif
