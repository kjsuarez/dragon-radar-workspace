#include "numpad.h"
#include <avr/io.h>

char keys[COLUMN_LENGTH][ROW_LENGTH] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

char current_input = 0;
char last_input = 0;

void initNumPadPins(KeyPin columnPins[], KeyPin rowPins[]){
    // set row pins as output
    for (uint8_t i = 0; i < ROW_LENGTH; i++){
        *(rowPins[i].ddr) |= (1 << rowPins[i].pin);
    }
    

    // set column pins as input
    // DDRD &= ~((1 << PD5) | (1 << PD6) | (1 << PD7));
    // DDRB &= ~(1 << PB0);
    for (uint8_t i = 0; i < COLUMN_LENGTH; i++){
        *(columnPins[i].ddr) &= ~(1 << columnPins[i].pin);
    }
    

    // set pull-up resistor on column pins
    // PORTD |= (1 << PD5) | (1 << PD6) | (1 << PD7);
    // PORTB |= (1 << PB0);
    for (uint8_t i = 0; i < COLUMN_LENGTH; i++){
        *(columnPins[i].port) |= (1 << columnPins[i].pin);
    }
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
            if (!(*(columnPins[x].input_pin_addr) & (1 << columnPins[x].pin)))
            {
                return (keys[y][x]);
            }
        }
        *(rowPins[y].port) |= (1 << rowPins[y].pin);
    }
    return 0;
}

uint8_t debounceNeeded(char current_input, char last_input){
    if (current_input == 0)
    {
        if (last_input == 0)
        {
            return 0;
        }
        if (last_input != 0)
        {
            return 1;
        }
    }
    if (current_input != 0)
    {
        if (last_input != current_input)
        {
            return 1;
        }
        if (last_input == current_input)
        {
            return 0;
        }
    }
    return 0;
}
