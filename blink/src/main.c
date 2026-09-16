#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
    DDRB |= (1 << PB1); // set the 5th, 6th, and 7th bits of DDRB to 1 to enable output on those pins
    while (1)
    {
        // Toggle PB5 (Exclusive OR) or set High/Low
        PORTB |= (1 << PB1); // Turn LED ON
        _delay_ms(100);      // Wait 500ms

        PORTB &= ~(1 << PB1); // Turn LED OFF
        _delay_ms(100);       // Wait 500ms
    }

    return 0;
}