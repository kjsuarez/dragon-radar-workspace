
#include <util/delay.h>
#include "numpad.h"
#include "st-7789.h"
#include "USART.h"
#include "main.h"
#include <avr/interrupt.h>
#include "nav.h"
#include "input-handler.h"

uint8_t timer_counter = 0;
// ?  ?  ?  ?  ?  cursor state  update cursor  update display
// 0, 0, 0, 0, 0, | 0           | 0            | 0
volatile uint8_t display_status = 0x8; //00001000
ISR(TIMER0_COMPA_vect)
{
    timer_counter ++;
    if (timer_counter > 20){
        timer_counter = 0;
        // toggle cursor state and set update bits
        display_status |= (1 << UPDATE_DISPLAY);
        display_status |= (1 << UPDATE_CURSOR);
        display_status ^= (1 << CURSOR_STATE);
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

int main(void)
{
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

    char current_position_buffer[LAT_BUFFER_SIZE] = {0};
    char lat_buffer[LAT_BUFFER_SIZE] = LAT_BUFFER_BASELINE;
    char lon_buffer[LON_BUFFER_SIZE] = LON_BUFFER_BASELINE;

    Nav nav = {
        {0, 0},
        {0, 0},
    };
    nav.current_position_buffer = current_position_buffer;
    nav.dest_lat_buffer = lat_buffer;
    nav.dest_lon_buffer = lon_buffer;
    nav.display_status = &display_status;

    initNumPadPins(columnPins, rowPins);
    initUSART();

    // initialize the Data/Control pin and Chip Select pin as outputs
    SPI_DDR |= (1 << DISPLAY_DC);
    initSpi();

    initDisplay();
    sei();
    init_timer();

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
        
        handle_input(debounced_press, &nav);
        handleDisplay(debounced_press, &nav);
    }
    return 0;
}