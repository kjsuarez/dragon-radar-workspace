#ifndef TEST
#include <avr/io.h>
#else
#include <stdint.h>
#endif

void floatToString(float value, char *output, uint32_t multiplier, uint8_t decimal_places);