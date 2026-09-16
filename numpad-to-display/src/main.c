#include <avr/io.h>
#include <util/delay.h>
#include "numpad.h"
#include "st-7789.h"
#include "USART.h"
#include <util/delay.h>
#include "main.h"
#include <avr/interrupt.h>

void handle_input(char button_press, Nav *nav){
    // enter
    // clear
    // .
    // 
}
char handleDisplay(char display_status, char data[]){
    if (display_status & 1)
    {
        display_status &= ~(1);
        printString(data);
        transmitByte('\n');
        displayStr(0, 0, WHITE, data);
    }
    return display_status;
}

uint8_t timer_counter = 0;
// ?  ?  ?  ?  ?  cursor state  update cursor  update display
// 0, 0, 0, 0, 0, | 0           | 0            | 0
volatile uint8_t display_status = 0;
ISR(TIMER0_COMPA_vect)
{
    timer_counter ++;
    if (timer_counter > 20){
        timer_counter = 0;
        display_status |= (1 << 0);
        display_status |= (1 << 1);
        display_status ^= (1 << 2);
    }
}

void init_timer(void){
    // set CTC mode
    TCCR0A |= (1 << WGM01);
    // set compare match value
    OCR0A = 249;
    // enable timer interrupt
    TIMSK0 |= (1 << OCIE0A);
    // set 1024 prescaler
    TCCR0B |= (1 << CS02) | (1 << CS00);
}

int main(void){
    KeyPin columnPins[COLUMN_LENGTH] = {
        {PD5, &PORTD, &PIND, &DDRD},
        {PD6, &PORTD, &PIND, &DDRD},
        {PD7, &PORTD, &PIND, &DDRD},
        {PB0, &PORTB, &PINB, &DDRB}};

    KeyPin rowPins[ROW_LENGTH] = {
        {PC5, &PORTC, &PINC, &DDRC},
        {PC4, &PORTC, &PINC, &DDRC},
        {PC3, &PORTC, &PINC, &DDRC},
        {PC2, &PORTC, &PINC, &DDRC}};

    char input_buffer[10] = {0};
    Nav nav = {
        {0, 0},
        {0, 0},
        0,
        0,
        input_buffer,
        0
    };
    initNumPadPins(columnPins, rowPins);
    initUSART();

    // initialize the Data/Control pin and Chip Select pin as outputs
    SPI_DDR |= (1 << DISPLAY_DC);
    initSpi();

    initDisplay();
    sei();
    init_timer();

    uint8_t buffer_i = 0;
    char debounced_press = 0;
    while (1){
        current_input = getButtonState(columnPins, rowPins);
        
        if(debounceNeeded(current_input, last_input)){
            _delay_ms(5);
            if (getButtonState(columnPins, rowPins) == current_input)
            {
                debounced_press = current_input;
            }
        }
        
        if (last_input != debounced_press){
            last_input = debounced_press;
            if (last_input != 0 && buffer_i < 9){
                input_buffer[buffer_i] = debounced_press; // last_input;
                buffer_i++;
                display_status |= 1;
            }
        }
        if (display_status & (1 << 1)){
            display_status &= ~(1 << 1);
            input_buffer[buffer_i] = display_status & (1 << 2) ? '_' : ' '; // last_input;
        }
        display_status = handleDisplay(display_status, input_buffer);
        // if (display_status & (1<<1)){
        //     display_status &= ~(1<<1);
        //     transmitByte('x');
        //     transmitByte('\n');
        // }
    }

    // while (1)
    // {
    //     if (display_status & 1){
    //         display_status &= ~(1);
    //         transmitByte('x');
    //         transmitByte('\n');
    //     }
        

    // }
    return 0;
}