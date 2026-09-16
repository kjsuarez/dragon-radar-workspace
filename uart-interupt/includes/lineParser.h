#ifndef TEST
    #include <avr/io.h>
#else
    #include <stdint.h>
#endif

#define POSITION_ID "$GPGLL"
#define COG_ID "$GPVTG"
#define ID_LEN 6
#define LAT_CHAR_POS 7
#define LAT_HEMI_POS 18
#define LON_CHAR_POS 20
#define LON_HEMI_POS 32

#define VALIDITY_CHAR_POS -6

typedef struct position{
    float longitude;
    float latitude;
    int course_over_ground;
    char status;
} Position;
// status: 1,         1,  0,0,0, 1
//         valid lat, valid lon, valid cog

void parseLine(char *line, Position *p, uint8_t line_length);