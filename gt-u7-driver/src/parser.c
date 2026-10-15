#include "parser.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// there's a state object that tracks 
// - if the message is valid (we saw char 0)
// - the message type (first field)
// - field index (current field being processed)
// - if the current field is used (based on message type and field index)

// given a char
// '$' start a new line (message is valid)
// ',' start a new field (increment field index) 
// '\n' if this is a used message, do biz logic
// everything else (if this is a known field, store the char in the designated field buffer)

GtU7LineState initGtU7LineState(void){
    GtU7LineState state = {0};
    return state;
}

Position lineToPosition(GtU7LineState *state){
    if (state->parsed){
        Position pos = {0};
        char *endptr;
        // latitude
        char lat_deg_buffer[3] = {0};
        strncpy(lat_deg_buffer, state->latBuffer, 2);
        uint8_t lat_degrees = (uint8_t)strtoul(lat_deg_buffer, &endptr, 10);
        double lat_minutes = strtod(&state->latBuffer[2], &endptr);
        lat_minutes = lat_degrees + (lat_minutes / 60);
        if (state->latHemi[0] == 'S'){
            lat_minutes *= -1;
        }
        pos.lat_dec_deg = lat_minutes;

        // longitude
        char lon_deg_buffer[4] = {0};
        strncpy(lon_deg_buffer, state->lonBuffer, 3);
        uint8_t lon_degrees = (uint8_t)strtoul(lon_deg_buffer, &endptr, 10);
        double lon_minutes = strtod(&state->lonBuffer[3], &endptr);
        lon_minutes = lon_degrees + (lon_minutes / 60);
        if (state->lonHemi[0] == 'W'){
            lon_minutes *= -1;
        }
        pos.lon_dec_deg = lon_minutes;

        pos.valid = 1;

        return pos;
    } else {
        Position blank = {0};
        return blank;
    }
}

char *setDestination(GtU7LineState *state){
    if (state->attrIndex == 0){
        return state->id;
    }
    if (state->attrIndex > 0){
        if (strcmp(state->id, "GPGLL") == 0){
            if (state->attrIndex == 1){
                return state->latBuffer;
            }
            if (state->attrIndex == 2){
                return state->latHemi;
            }
            if (state->attrIndex == 3){
                return state->lonBuffer;
            }
            if (state->attrIndex == 4){
                return state->lonHemi;
            }
        }
        return 0;
    }
    return 0;
}

void processGtU7Char(GtU7LineState *state, char input_char){
    char *dest = 0;
    if (input_char == '$'){
        state->attrIndex = 0;
        state->attrCharIndex = 0;
        state->online = 1;
        return;
    }
    if (state->online != 1){
        return;
    }
    switch(input_char) {
        case ',':
            dest = setDestination(state);
            
            if (dest){
                for (size_t i = 0; i < state->attrCharIndex; i++){
                    dest[i] = state->attrBuffer[i];
                }
                dest[state->attrCharIndex] = '\0';
            }
            for (size_t i = 0; i < state->attrCharIndex; i++){
                state->attrBuffer[i] = 0;
            }

            state->attrIndex += 1;
            state->attrCharIndex = 0;

            if (strcmp(state->id, "GPGLL") == 0){
                state->parsed = 0;
            }
            
            break;
        case '\n':
            state->online = 0;
            state->attrIndex = 0;
            state->attrCharIndex = 0;
            uint8_t buffer_i = 0;
            while(state->attrBuffer[buffer_i] !=  '\0'){
                state->attrBuffer[buffer_i] = 0;
                buffer_i += 1;
            }
            if (strcmp(state->id, "GPGLL") == 0){
                state->parsed = 1;
            }
            
            break;
        default:
            state->attrBuffer[state->attrCharIndex] = input_char;
            state->attrCharIndex++;
            state->attrBuffer[state->attrCharIndex] = '\0';
            break;
    }
    return;
}
