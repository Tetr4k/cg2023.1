#include "transforms.h"

main(){
	mat4 P = perspective(45, 3, 1.7, 100.4);
	print(P);
}