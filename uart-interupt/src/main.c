#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"

char line_buffer[50];
char line[50];
uint8_t line_buffer_index = 0;
uint8_t line_state = 0;


ISR(USART_RX_vect){
    char x = UDR0;
    line_buffer[line_buffer_index] = x;
    line_buffer_index++;
    if (x == '\n' || line_buffer_index >= 50){
        line_buffer[line_buffer_index] = 0; // Null terminate the string
        for (int i = 0; i < line_buffer_index; i++){
            line[i] = line_buffer[i];
        }
        line_buffer_index = 0; // Reset the buffer index for the next line
        line_state = (1 << 0); // Set the line ready flag
    }
}

int main(void){
    initUSART();
    // initIntUSART();
    // sei();

    while (1)
    {
        // if(line_state | 1 << 0){
        //     // processLine(line);
        // }
        // x = receiveByte();
        transmitByte('x');
    }
    
}