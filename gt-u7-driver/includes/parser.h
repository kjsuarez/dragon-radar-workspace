#include <stdint.h>
#include <math.h>

typedef struct GtU7LineState{

    uint8_t online;
    uint8_t parsed;
    char attrBuffer[12];
    uint8_t attrIndex;
    uint8_t attrCharIndex;
    char id[6];
    char latBuffer[12];
    char lonBuffer[12];
    char latHemi[2];
    char lonHemi[2];
} GtU7LineState;

typedef struct position{
    // uint16_t longDegrees;
    // uint32_t longMinutes;
    // char longDir;
    // uint16_t latDegrees;
    // uint32_t latMinutes;
    // char latDir;
    float lat_dec_deg;
    float lon_dec_deg;
    uint8_t valid;

} Position;

typedef struct GtU7State{
    GtU7LineState lineState;
    Position position;
} GtU7State;

GtU7LineState initGtU7LineState(void);
void processGtU7Char(GtU7LineState *state, char input_char);
Position lineToPosition(GtU7LineState *state);