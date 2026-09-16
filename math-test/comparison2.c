#include <stdio.h>

#include "trigUtility.h"
// #include "cordic-8.h"
// #include "floatUtility.h"
#include <math.h>
#define XMUL 64.000000

int main(void)
{
    // int s, c;
    int i;
    double p;
    // float degrees = 45.0;
    float radians;
    double sine;
    double cosine;

    // cordic
    int s, c;

    // float x = 1.23456;
    char angle_str[50];

    char buffer[50];

    for (i = 0; i < 360; i++){
        radians = (double)i * (M_PI/180);
        // cordic((radians * MUL), &s, &c, 8);
        // printf("Angle:%d, Radians:%.6f, Sin:%.6f, cordic-sin: %.6f \n", i, radians, sin(radians), s/MUL);


        fullCircleCordic(i, &sine, &cosine);
        // cordicPassthrough(radians, &s, &c);
        printf("Angle:%d, Cordic degrees:%d, Radians:%.6f, Sine:%.6f, Cordic Sine:%.6f\n", i, cordicDegrees(i), radians, sin(radians), sine);
    }
}