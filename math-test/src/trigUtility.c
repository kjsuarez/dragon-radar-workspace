// #include "cordic-8.h"
#include "cordic-16.h"
#include <math.h>
#ifndef TEST

#else
    #include <stdio.h>
#endif

// #define M_PI 3.14159265359
#define earth_radius 637100 //in meters

float degreeToRadian(float angle){
    return (angle * (M_PI / 180.0));
}

int cordicDegrees(int angle){
    if(angle > 90 && angle < 270){
        angle = 180 - angle;
    }else if (angle >= 270){
        angle = angle - 360;
    }
    return angle;
}

void fullCircleCordic(int angle, double *sine,  double *cosine){
    float radian;
    int s, c;
    int cordic_angle = cordicDegrees(angle);
    radian = degreeToRadian(cordic_angle);
    cordic((int)(radian * MUL), &s, &c, 16);
    if (angle > 90 && angle < 270){
        s = -s;
        c = -c;
    }
    
    
    // printf("cordic Angle:%d, cordic Radians:%.6f \n", angle, radian);
    

    *sine = (double)s/MUL;
    *cosine = (double)c/MUL;
}

void cordicPassthrough(int theta, int *s, int *c){
    cordic((theta*MUL), s, c, 8);
}

// φ: latitude in radians, λ: longitude in radians

// Rhumb line bearing
// Δψ = ln(tan(π / 4 + φ2 / 2) / tan(π / 4 + φ1 / 2))
// θ = atan2(Δλ, Δψ)
void rhumbBearing(float delta_lon, float delta_iso_lat, float *bearing){    
    *bearing = atan2(delta_lon, delta_iso_lat);
}

void rhumbDistance(float start_lat, float delta_ios_lat, float delta_lat, float delta_lon, int *distance){
    float q = delta_ios_lat == 0 ? cos(start_lat) : delta_lat/delta_ios_lat;
    *distance = sqrt(delta_lat * delta_lat + q * q * delta_lon * delta_lon) * earth_radius;
}

void calculateRhumb(float start_lat, float start_long, float end_lat, float end_long, float *bearing, int *distance){
    start_lat = degreeToRadian(start_lat);
    start_long = degreeToRadian(start_long);
    end_lat = degreeToRadian(end_lat);
    end_long = degreeToRadian(end_long);
    float delta_lon = end_long - start_long;
    if (fabsf(delta_lon) > M_PI){
        delta_lon = delta_lon > 0 ? delta_lon - 2 *M_PI : delta_lon + 2 * M_PI;
    }
    float delta_iso_lat = log(tan(M_PI / 4 + end_lat / 2) / tan(M_PI / 4 + start_lat / 2));
    // printf("start lat radians:%.6f, start lon radians:%.6f, end lat radians:%.6f, end lon radians:%.6f, \n", start_lat, start_long, end_lat, end_long);
    // printf("Δλ: %.6f\n", delta_lon);
    rhumbBearing(delta_lon, delta_iso_lat, bearing);
    float delta_lat = end_lat - start_lat;
    rhumbDistance(start_lat, delta_iso_lat, delta_lat, delta_lon, distance);
}