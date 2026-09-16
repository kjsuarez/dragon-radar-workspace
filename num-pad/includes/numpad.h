#include <avr/io.h>

#define COLUMN_LENGTH 4
#define ROW_LENGTH 4

extern char keys[COLUMN_LENGTH][ROW_LENGTH];

typedef struct keyPin
{
    uint8_t pin;
    volatile uint8_t *port;
} KeyPin;


extern char button_state;   // the button state as of this loop
extern char pressed_button; // The button currently being pressed

char getButtonState();
void initNumPadPins();