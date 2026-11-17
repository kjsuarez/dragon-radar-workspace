#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include <stdio.h>
// #include "cordic-8.h"
#include "floatUtility.h"
#include "rhumb.h"

int main(void){
    char bearing_buffer[40];
    float bearing;
    int distance;
    char distance_buffer[20];

    while (1){
        calculateRhumb(45.0, -122.0, 45.0, -122.6, &bearing, &distance);
        snprintf(bearing_buffer, sizeof(bearing_buffer), "%d", (int)(bearing * (180.0 / M_PI)));
        snprintf(distance_buffer, sizeof(distance_buffer), "%d", distance);
        printString("bearing: ");
        printString(bearing_buffer);
        printString(", distance: ");
        printString(distance_buffer);
        transmitByte('\n');
    }
}