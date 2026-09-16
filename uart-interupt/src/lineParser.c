#include "lineParser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

float parseCoor(char *line, uint8_t coor_i, uint8_t hemi_i, uint8_t degrees_len){
    char tmp_minute = line[coor_i + degrees_len];
    line[coor_i + degrees_len] = '\0';
    int degrees = (int)strtol(line + coor_i, NULL, 10);
    printf("Degrees: %d\n", degrees);
    line[coor_i + degrees_len] = tmp_minute;
    float minutes = (float)strtod(line + coor_i + degrees_len, NULL);
    minutes = degrees + (minutes / 60);
    if (line[hemi_i] == 'S' || line[hemi_i] == 'W'){
        minutes = minutes * -1;
    }
    return minutes;
}

void parseLine(char *line, Position *p, uint8_t line_length){
    printf("HERE\n");
    if(strncmp(line, POSITION_ID, ID_LEN) == 0){
        printf("correct id\n");
        printf("validity char: %c\n", line[line_length - 6]);
        if (line[line_length VALIDITY_CHAR_POS] == 'A'){
            printf("valid line\n");
            p->latitude = parseCoor(line, LAT_CHAR_POS, LAT_HEMI_POS, 2);
            p->status |= (1 << 7); //set latitude to valid

            p->longitude = parseCoor(line, LON_CHAR_POS, LON_HEMI_POS, 3);
            p->status |= (1 << 6); // set latitude to valid
        }else{
            p->status &= ~(0 << 7); // set latitude to invalid
        }
    } else if (strncmp(line, COG_ID, ID_LEN) == 0){
        
    }
}