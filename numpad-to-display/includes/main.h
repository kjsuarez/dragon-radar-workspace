#include <avr/io.h>

#define LAT_BUFFER_SIZE 10
#define LON_BUFFER_SIZE 11

// Display Status Bit Definitions
#define CURSOR_STATE 2
#define UPDATE_CURSOR 1
#define UPDATE_DISPLAY 0

typedef struct position{
    float longitude;
    float latitude;
   
} Position;

typedef struct nav{
    Position current_position;
    Position destination;
    int course_over_ground;
    int distance_to_destination;
    char *current_position_buffer;
    char *destination_buffer;
    volatile uint8_t *display_status;
    uint8_t buffer_i;
    char status;
    char cursor_char;
} Nav;
