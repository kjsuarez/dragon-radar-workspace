#include "nav.h"

uint8_t latIndex(uint8_t buffer_i);
uint8_t setLatIndex(uint8_t buffer_i, uint8_t lat_index);
uint8_t lonIndex(uint8_t buffer_i);
uint8_t setLonIndex(uint8_t buffer_i, uint8_t lon_index);
uint8_t increment_relevant_index(Nav *nav, int8_t amount);
void handle_input(char button_press, Nav *nav);
void handleDisplay(char button_press, Nav *nav);
extern void handleEnter(Nav *nav);
void handleClear(Nav *nav);
void handleOpposite(Nav *nav);
void handleBckSpc(Nav *nav);
void handleClear(Nav *nav);
void handleEnter(Nav *nav);