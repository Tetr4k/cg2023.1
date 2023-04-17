#include "Color.h"

int main(){
    RGB A = 0xff2d5a_rgb;
    RGB B = blue;
    RGB C = lerp(0.1, A, B);
    print("A", toVec(A));
    print("B", toVec(B));
    print("C", toVec(C));
} 