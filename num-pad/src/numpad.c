#include "numpad.h"

char keys[COLUMN_LENGTH][ROW_LENGTH] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

char button_state = 0;
char pressed_button = 0;

void initNumPadPins(){
    // set row pins as output
    DDRC |= (1 << PC5) | (1 << PC4) | (1 << PC3) | (1 << PC2);

    // set column pins as input
    DDRD &= ~((1 << PD5) | (1 << PD6) | (1 << PD7));
    DDRB &= ~(1 << PB0);

    // set pull-up resistor on column pins
    PORTD |= (1 << PD5) | (1 << PD6) | (1 << PD7);
    PORTB |= (1 << PB0);
}

char getButtonState(KeyPin columnPins[COLUMN_LENGTH], KeyPin rowPins[ROW_LENGTH])
{
    for (uint8_t y = 0; y < ROW_LENGTH; y++)
    {
        *(rowPins[y].port) |= (1 << rowPins[y].pin);
    }

    for (uint8_t y = 0; y < ROW_LENGTH; y++)
    {
        *(rowPins[y].port) &= ~(1 << rowPins[y].pin);
        for (uint8_t x = 0; x < COLUMN_LENGTH; x++)
        {
            if (!(*(columnPins[x].port) & (1 << columnPins[x].pin)))
            {
                return (keys[y][x]);
            }
        }
        *(rowPins[y].port) |= (1 << rowPins[y].pin);
    }
    return 0;
}