#include <avr/io.h>

#define LAT_BUFFER_SIZE 10
#define LON_BUFFER_SIZE 11

// Display Status Bit Definitions
#define NAV_MODE 4 // 0 = edit, 1 = navigate
#define LATLON_FOCUS 3 // 0 = lat, 1 = lon
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
    char *dest_lat_buffer;
    char *dest_lon_buffer;
    volatile uint8_t *display_status;
    uint8_t buffer_i;
    char status;
    char cursor_char;
} Nav;

uint8_t latIndex(uint8_t buffer_i);
uint8_t setLatIndex(uint8_t buffer_i, uint8_t lat_index);
uint8_t lonIndex(uint8_t buffer_i);
uint8_t setLonIndex(uint8_t buffer_i, uint8_t lon_index);
uint8_t increment_relevant_index(Nav *nav);
void handleEnter(Nav *nav);