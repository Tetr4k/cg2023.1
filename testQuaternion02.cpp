#include "Quaternion.h"

int main(){
	vec3 u = {3, 4, 5};
	float theta = 0.3;
	vec3 v = {2, 1, 4};

	vec3 n = normalize(u);

	Quaternion Q = cos(theta/2) + sin(theta/2)*n;
	Quaternion Qi = Q.a - Q.v;

	print(Q*v*Qi);
}