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
    if (nav->dest_lat_buffer[0] != '-'){
        for (uint8_t i = LAT_BUFFER_SIZE - 1; i-- > 0;){
            nav->dest_lat_buffer[i + 1] = nav->dest_lat_buffer[i];
        }
        nav->dest_lat_buffer[0] = '-';
        nav->buffer_i++;
    } else { // remove '-'
        if (nav->buffer_i > 0)
        {
            nav->buffer_i--;
        }

        for (uint8_t i = 0; i < LAT_BUFFER_SIZE; i++){
            nav->dest_lat_buffer[i] = nav->dest_lat_buffer[i + 1];
        }

        nav->dest_lat_buffer[LAT_BUFFER_SIZE] = ' ';
    }
    *(nav->display_status) |= 1;
}

void handleBckSpc(Nav *nav){
    if (nav->buffer_i > 0){
        nav->dest_lat_buffer[nav->buffer_i] = '0';
        if (nav->dest_lat_buffer[nav->buffer_i - 1] == '-'){
            return;
        }
        nav->buffer_i--;
        // Always skip over decimal point
        if (nav->dest_lat_buffer[nav->buffer_i] == '.'){
            nav->buffer_i--;
        }
        // nav->dest_lat_buffer[nav->buffer_i] = '0';
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
            nav->dest_lat_buffer[nav->buffer_i] = button_press;
            nav->buffer_i++;
            // Always skip over decimal point
            if (nav->dest_lat_buffer[nav->buffer_i] == '.'){
                nav->buffer_i++;
            }
            *(nav->display_status) |= 1;
        }
    }
}

void handleDisplay(char button_press, Nav *nav){
    if (*(nav->display_status) & 1){
        *(nav->display_status) &= ~(1);
            
        if (*(nav->display_status) & (1 << UPDATE_CURSOR)){
            *(nav->display_status) &= ~(1 << UPDATE_CURSOR);

            // if cursor needs updating and cursor state is 1
            if (*(nav->display_status) & (1 << CURSOR_STATE)){
                if (nav->buffer_i > 0){
                    displaySubstr(0, 0, WHITE, BLACK, 0, nav->buffer_i, nav->dest_lat_buffer);
                }

                displayColorChar(nav->buffer_i * 8, 0, BLACK, WHITE, nav->dest_lat_buffer[nav->buffer_i]);

                if (nav->buffer_i < LAT_BUFFER_SIZE){
                    displaySubstr(0, 0, WHITE, BLACK, nav->buffer_i + 1, LAT_BUFFER_SIZE, nav->dest_lat_buffer);
                }

                displayStr(LAT_BUFFER_SIZE * 8, 0, WHITE, ",");
                displayStr((LAT_BUFFER_SIZE + 1) * 8, 0, WHITE, nav->dest_lon_buffer);
                return;
            }
        }
        displayStr(0, 0, WHITE, nav->dest_lat_buffer);
        displayStr(LAT_BUFFER_SIZE * 8, 0, WHITE, ",");
        displayStr((LAT_BUFFER_SIZE + 1) * 8, 0, WHITE, nav->dest_lon_buffer);
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

uint8_t latIndex(uint8_t buffer_i){
    return buffer_i >> 4;
}

uint8_t setLatIndex(uint8_t buffer_i, uint8_t lat_index){
    return (buffer_i & 0xF0) | ((lat_index & 0xF) << 4);
}
uint8_t lonIndex(uint8_t buffer_i){
    return buffer_i & 0xF;
}

uint8_t setLonIndex(uint8_t buffer_i, uint8_t lon_index){
    return buffer_i & lon_index;
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
    char lat_buffer[10] = "00.000000";
    char lon_buffer[11] = "000.000000";

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