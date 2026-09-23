#include <avr/io.h>
#include <util/delay.h>
#include "numpad.h"
#include "st-7789.h"
#include "USART.h"
#include <util/delay.h>
#include "main.h"
#include <avr/interrupt.h>

void handleOpposite(Nav *nav){
    // add '-'
    if (nav->destination_buffer[0] != '-') {
        nav->destination_buffer[nav->buffer_i] = ' ';
        for (uint8_t i = nav->buffer_i; i-- > 0;){
            nav->destination_buffer[i + 1] = nav->destination_buffer[i];
            nav->destination_buffer[i] = ' ';
        }
        nav->destination_buffer[0] = '-';
        nav->buffer_i++;
    } else { // remove '-'
        // nav->destination_buffer[nav->buffer_i] = ' ';
        nav->buffer_i--;

        for (uint8_t i = 0; i < LAT_BUFFER_SIZE; i++){
            nav->destination_buffer[i] = nav->destination_buffer[i + 1];
        }
        nav->destination_buffer[nav->buffer_i] = ' ';
        nav->destination_buffer[nav->buffer_i + 1] = ' ';
    }
    *(nav->display_status) |= 1;
}

void handleBckSpc(Nav *nav){
    if (nav->buffer_i > 0){
        nav->destination_buffer[nav->buffer_i] = ' ';
        nav->buffer_i--;
        nav->destination_buffer[nav->buffer_i] = ' ';
    }
}

void handle_input(char button_press, Nav *nav){
    // enter
    // clear
    // .
    // - 

    if (last_input != button_press){
        last_input = button_press;
        // minus sign: toggles '-' at the front of the buffer
        if (last_input == 'A'){
            handleOpposite(nav);
            return;
        }
        if (last_input == 'B'){
            handleBckSpc(nav);
            return;
        }
        if (button_press != 0 && nav->buffer_i < 9)
        {
            nav->destination_buffer[nav->buffer_i] = button_press;
            nav->buffer_i++;
            *(nav->display_status) |= 1;
        }
    }
}

void handleDisplay(char button_press, Nav *nav){
    if (*(nav->display_status) & 1){
        *(nav->display_status) &= ~(1);
        printString(nav->destination_buffer);
        transmitByte('\n');

        if (*(nav->display_status) & (1 << UPDATE_CURSOR)){
            *(nav->display_status) &= ~(1 << UPDATE_CURSOR);

            // if cursor needs updating and cursor state is 1
            if (*(nav->display_status) & (1 << CURSOR_STATE))
            {
                if (nav->buffer_i > 0){
                    displaySubstr(0, 0, WHITE, BLACK, 0, nav->buffer_i, nav->destination_buffer);
                }

                displayColorChar(nav->buffer_i * 8, 0, BLACK, WHITE, nav->destination_buffer[nav->buffer_i]);

                if (nav->buffer_i < LAT_BUFFER_SIZE){
                    displaySubstr(0, 0, WHITE, BLACK, nav->buffer_i + 1, LAT_BUFFER_SIZE, nav->destination_buffer);
                }
                return;
            }
        }
        displayStr(0, 0, WHITE, nav->destination_buffer);
    }
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

    char current_position_buffer[LAT_BUFFER_SIZE] = {0};
    char input_buffer[10] = "00.000000";

    Nav nav = {
        {0, 0},
        {0, 0},
    };
    nav.current_position_buffer = current_position_buffer;
    nav.destination_buffer = input_buffer;
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
    return 0;
}