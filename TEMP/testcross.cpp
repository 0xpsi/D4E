
#include <stdio.h>
#include <cmath>
#include <vect4.h>

bool ortho(vect4 a, vect4 b){
    return a*b==0;
}

int main(){
    bool z = true;
    vect4 v1(1,3,-4,-0.003);
    vect4 v2(-3,1,4.4343,3);
    vect4 v3(1,2.005,0,5);
    vect4 v4 = cross4(v1,v2,v3);
    //vect4 v4(1,1,1,0);
    if(!ortho(v4,v1)){z=false;}
    if(!ortho(v4,v2)){z=false;}
    if(!ortho(v4,v3)){z=false;}

    printf("Ortho: %s\n", (z) ? "TRUE" : "FALSE");

    v1.print("v1: ", "\n");
    v2.print("v2: ", "\n");
    v3.print("v3: ", "\n");
    v4.print("cross: ", "\n");

    printf("v3 mag: %0.8f | v4 mag: %0.8f\n", v3.mag(), v4.mag());

    return 0;
}
