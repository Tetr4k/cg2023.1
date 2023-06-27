#version 330

uniform sampler2D tex;

in vec2 texCoords;

out vec4 fragColor;

vec4 erosion_filter(){
	vec4 sampleTex[5];
	ivec2 pos = ivec2(gl_FragCoord.xy);
	sampleTex[0] = texelFetch(tex, pos + ivec2(0, 0), 0);
	sampleTex[1] = texelFetch(tex, pos + ivec2(1, 0), 0);
	sampleTex[2] = texelFetch(tex, pos + ivec2(-1, 0), 0);
	sampleTex[3] = texelFetch(tex, pos + ivec2(0, 1), 0);
	sampleTex[4] = texelFetch(tex, pos + ivec2(0, -1), 0);

	bool inside[5];
	inside[0] = sampleTex[0].r > 0;
	inside[1] = sampleTex[1].r > 0;
	inside[2] = sampleTex[2].r > 0;
	inside[3] = sampleTex[3].r > 0;
	inside[4] = sampleTex[4].r > 0;

	if(inside[0] && inside[1] && inside[2] && inside[3] && inside[4])
		return vec4(1, 1, 1, 1);

	return vec4(0, 0, 0, 1);
}

vec4 dilation_filter(){
	vec4 sampleTex[5];
	ivec2 pos = ivec2(gl_FragCoord.xy);
	sampleTex[0] = texelFetch(tex, pos + ivec2(0, 0), 0);
	sampleTex[1] = texelFetch(tex, pos + ivec2(1, 0), 0);
	sampleTex[2] = texelFetch(tex, pos + ivec2(-1, 0), 0);
	sampleTex[3] = texelFetch(tex, pos + ivec2(0, 1), 0);
	sampleTex[4] = texelFetch(tex, pos + ivec2(0, -1), 0);

	bool inside[5];
	inside[0] = sampleTex[0].r > 0;
	inside[1] = sampleTex[1].r > 0;
	inside[2] = sampleTex[2].r > 0;
	inside[3] = sampleTex[3].r > 0;
	inside[4] = sampleTex[4].r > 0;

	if(inside[0] || inside[1] || inside[2] || inside[3] || inside[4])
		return vec4(1, 1, 1, 1);
		
	return vec4(0, 0, 0, 1);
}

void main(){
	fragColor = dilation_filter();
	//fragColor = erosion_filter();
}
