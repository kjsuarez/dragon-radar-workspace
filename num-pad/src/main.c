#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"

#include "numpad.h"

int main(void){
    KeyPin columnPins[COLUMN_LENGTH] = {
        {PD5, &PIND}, 
        {PD6, &PIND}, 
        {PD7, &PIND}, 
        {PB0, &PINB}
    };

    KeyPin rowPins[ROW_LENGTH] = {
        {PC5, &PORTC}, 
        {PC4, &PORTC}, 
        {PC3, &PORTC}, 
        {PC2, &PORTC}
    };

    initUSART();
    initNumPadPins();

    while (1)
    {
        transmitByte('X');
        button_state = getButtonState(columnPins, rowPins);
        if(pressed_button != button_state){
            pressed_button = button_state;
            if (pressed_button != 0){
                transmitByte(pressed_button);
            }
        }


        // for (uint8_t y = 0; y < ROW_LENGTH; y++)
        // {
        //     *(rowPins[y].port) &= ~(1 << rowPins[y].pin);
        //     for (uint8_t x = 0; x < COLUMN_LENGTH; x++)
        //     {
        //         if (!(*(columnPins[x].port) & (1 << columnPins[x].pin)))
        //         {
        //             transmitByte(keys[y][x]);
        //         }
        //     }
        //     *(rowPins[y].port) |= (1 << rowPins[y].pin);
        // }
    }
    return 0;
}