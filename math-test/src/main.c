#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include <stdio.h>
// #include "cordic-8.h"
#include "floatUtility.h"
#include "trigUtility.h"

int main(void){

    initUSART();
    // double sine;
    // double cosine;
    // char degree_buffer[6];
    char bearing_buffer[40];
    float bearing;
    int distance;
    char distance_buffer[20];

    while (1){
        // for (size_t i = 0; i < 360; i++){
        //     fullCircleCordic(i, &sine, &cosine);
        //     floatToString(sine, buffer, 100000, 5);
        //     snprintf(degree_buffer, sizeof(degree_buffer), "%d", i);

        //     printString(degree_buffer);
        //     printString(": ");
        //     printString(buffer);
        //     transmitByte('\n');
        // }

        calculateRhumb(45.0, -122.0, 45.0, -122.6, &bearing, &distance);
        // floatToString((int)(bearing * (180.0 / M_PI)), bearing_buffer, 100000, 5);
        snprintf(bearing_buffer, sizeof(bearing_buffer), "%d", (int)(bearing * (180.0 / M_PI)));
        snprintf(distance_buffer, sizeof(distance_buffer), "%d", distance);
        printString("bearing: ");
        printString(bearing_buffer);
        printString(", distance: ");
        printString(distance_buffer);
        transmitByte('\n');

        // printString("fart zone");
        // transmitByte('\n');
    }
}