#include <avr/io.h>

#define COLUMN_LENGTH 4
#define ROW_LENGTH 4

extern char keys[COLUMN_LENGTH][ROW_LENGTH];

typedef struct keyPin
{
    uint8_t pin;
    volatile uint8_t *port;
    volatile uint8_t *input_pin_addr;
    volatile uint8_t *ddr;
} KeyPin;


extern char current_input;   // the button state as of this loop
extern char last_input; // The button currently being pressed

char getButtonState();
void initNumPadPins(KeyPin columnPins[], KeyPin RowPins[]);
uint8_t debounceNeeded(char current_input, char last_input);