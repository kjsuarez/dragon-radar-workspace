#include "parser.h"
#include <math.h>
#include <string.h>
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
    GtU7LineState state;
    state.charIndex = -1;
    return state;
}

GtU7LineState processGtU7Char(GtU7LineState state, char input_char){
    switch(input_char) {
        case '$':
            state.charIndex = 0;
            break;
        case ',':
            state.charIndex++;
            break;
        case '\n':
            if (state.charIndex != -1 && strcmp(state.messageType, "GPGLL") == 0)
            {
                // setCoor(&state);
            }
            break;
        default:
            // everything else (if this is a known field, store the char in the designated field buffer)
            break;
    }
    return state;
}
