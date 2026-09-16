#include <stdint.h>

typedef struct GtU7LineState
{
    char charBuffer;
    int8_t charIndex;
    char messageType[5];
    char longBuffer[11];
    char longDir;
    char latBuffer[11];
    char latDir;
} GtU7LineState;

typedef struct position{
    uint16_t longDegrees;
    uint32_t longMinutes;
    char longDir;
    uint16_t latDegrees;
    uint32_t latMinutes;
    char latDir;
} Position;

typedef struct GtU7State
{
    GtU7LineState lineState;
    Position position;
    uint16_t courseOverGround;
} GtU7State;

GtU7LineState initGtU7LineState(void);
int32_t bearing(int32_t start_lat, int32_t start_lon, int32_t end_lat, int32_t end_lon);