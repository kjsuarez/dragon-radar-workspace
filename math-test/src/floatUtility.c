#include "floatUtility.h"
#include <string.h>
#include <stdio.h>

uint8_t decimalPointIndex(char *int_str, uint8_t multiplier_places){
    uint8_t str_places = strlen(int_str);
    if (str_places <= multiplier_places){
        return 1;
    }
    else{
        return str_places - multiplier_places;
    }
}

uint8_t countLeadingZeros(char *int_str, uint8_t decimal_places){
    uint8_t str_places = strlen(int_str);
    if(str_places <= decimal_places){
        return decimal_places - str_places;
    } else {
        return 0;
    }
}

void floatToString(float value, char *output, uint32_t multiplier, uint8_t decimal_places)
{
    long multiplied_value = (long)(value * multiplier);
    uint8_t is_negative = multiplied_value < 0 ? 1 : 0;
    char int_string[10];
    snprintf(int_string, sizeof(int_string), "%ld", multiplied_value);
    int int_size = strlen(int_string);
    uint8_t decimal_point_index = decimalPointIndex(int_string, decimal_places);
    
    uint8_t float_index = 0;
    uint8_t int_index = 0;

    //handle negative values
    if (multiplied_value < 0){
        output[float_index++] = '-';
        int_index++;
    }

    // handle leading zeros and decimal point
    if ((int_size - is_negative) <= decimal_places){
        uint8_t zero_count = countLeadingZeros(int_string, decimal_places);
        output[float_index++] = '0';
        output[float_index++] = '.';
        for (int i = 0; i < zero_count; i++)
        {
            output[float_index++] = '0';
        }
    }
    while (int_string[int_index] != '\0'){
        if (float_index == decimal_point_index){
            output[float_index++] = '.';
        } else {
            output[float_index++] = int_string[int_index++];
        }   
    }
    output[float_index] = '\0';
}