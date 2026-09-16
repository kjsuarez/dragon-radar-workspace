#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"

int main(void){
    char serialCharacter;
    LED_DDR = 0xff;
    DDRB |= (1 << PB1);
    initUSART();
    printString("Hello World!\r\n");
    while (1)
    {
        serialCharacter = receiveByte();
        PORTB ^= (1<<PB1);
        transmitByte(serialCharacter);
        // LED_PORT = serialCharacter;
        // PORTB |= (1 << PB1);
    }

    return 0;
}