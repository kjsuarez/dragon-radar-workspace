
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
    char status;
} Nav;

// typedef struct display_data{
//     char *current_position_buffer;
//     char *destination_buffer;
// } DisplayData;