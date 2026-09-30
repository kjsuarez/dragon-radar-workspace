#include <avr/io.h>
#include <util/delay.h>
#include "numpad.h"
#include "st-7789.h"
#include "USART.h"
#include <util/delay.h>
#include "main.h"
#include <avr/interrupt.h>

void handleOpposite(Nav *nav){
    uint8_t relevant_index = 0;
    char *active_buffer;
    uint8_t max_length;
    // is it for lat buffer or lon buffer?
    if (!(*(nav->display_status) & (1 << LATLON_FOCUS)))
    {
        relevant_index = latIndex(nav->buffer_i);
        active_buffer = nav->dest_lat_buffer;
        max_length = LAT_BUFFER_SIZE;
    }
    else
    {
        relevant_index = lonIndex(nav->buffer_i);
        active_buffer = nav->dest_lon_buffer;
        max_length = LON_BUFFER_SIZE;
    }

    // add '-'
    if (active_buffer[0] != '-'){
        for (uint8_t i = max_length - 1; i-- > 0;){
            active_buffer[i + 1] = active_buffer[i];
        }
        active_buffer[0] = '-';
        relevant_index = setLatIndex(relevant_index, latIndex(relevant_index) + 1);
    } else { // remove '-'
        if (latIndex(relevant_index) > 0)
        {
            relevant_index = setLatIndex(relevant_index, latIndex(relevant_index) - 1);
        }

        for (uint8_t i = 0; i < max_length; i++){
            active_buffer[i] = active_buffer[i + 1];
        }

        active_buffer[max_length] = ' ';
    }    
    *(nav->display_status) |= 1;
}

void handleBckSpc(Nav *nav){
    uint8_t relevant_index = 0;
    char *active_buffer;
    // is it for lat buffer or lon buffer?
    if (!(*(nav->display_status) & (1 << LATLON_FOCUS)))
    {
        relevant_index = latIndex(nav->buffer_i);
        active_buffer = nav->dest_lat_buffer;
    }
    else
    {
        relevant_index = lonIndex(nav->buffer_i);
        active_buffer = nav->dest_lon_buffer;
    }
    if (relevant_index > 0){
        active_buffer[relevant_index] = '0';
        if (active_buffer[relevant_index - 1] == '-'){
            return;
        }
        relevant_index--;
        // Always skip over decimal point
        if (active_buffer[relevant_index] == '.'){
            relevant_index--;
        }
    }
}

void handleDigit(Nav *nav, char button_press){
    printString("handle digit");
    printString("\n");
    uint8_t relevant_index = 0;
    char *active_buffer;
    uint8_t max_length;
    // is it for lat buffer or lon buffer?
    if (!(*(nav->display_status) & (1 << LATLON_FOCUS)))
    {
        relevant_index = latIndex(nav->buffer_i);
        active_buffer = nav->dest_lat_buffer;
        max_length = LAT_BUFFER_SIZE;
    }else{
        relevant_index = lonIndex(nav->buffer_i);
        active_buffer = nav->dest_lon_buffer;
        max_length = LON_BUFFER_SIZE;
    }
    active_buffer[relevant_index] = button_press;

    printString("relevant_index: ");
    printByte(relevant_index);
    printString(", ");
    printString("max length: ");
    printByte(max_length);
    printString("\n");
    if (relevant_index >= max_length - 1)
    {
        *(nav->display_status) |= 1;
        handleEnter(nav);
    }

    // relevant_index++;
    relevant_index = increment_relevant_index(nav);
    // Always skip over decimal point
    if (active_buffer[relevant_index] == '.'){
        // relevant_index++;
        relevant_index = increment_relevant_index(nav);
    }
    
    *(nav->display_status) |= 1;
}

void handleEnter(Nav *nav){
    printString("handle enter\n");
    // if edit mode is off turn it on
    if (*(nav->display_status) & (1 << NAV_MODE)){
        printString("activate edit mode\n");
        *(nav->display_status) &= ~(1 << NAV_MODE);
        return;
    }
    // if you're on lon turn off edit mode
    if (*(nav->display_status) & (1 << LATLON_FOCUS)){
        printString("activate nav mode\n");
        *(nav->display_status) &= ~(1 << LATLON_FOCUS);
        *(nav->display_status) &= ~(1 << NAV_MODE);
        return;
    }
    // if you're on lat, move to lon
    if (*(nav->display_status) & ~(1 << LATLON_FOCUS)){
        printString("toggle focus to longitude\n");
        *(nav->display_status) |= (1 << LATLON_FOCUS);
    }
}

void handle_input(char button_press, Nav *nav){
    // enter
    // clear
    // -      ✓
    // bckspc ✓
    // digit  

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
        if (button_press != 0){
            printString("registered number press\n");
            handleDigit(nav, button_press);
        }
    }
}

void handleDisplay(char button_press, Nav *nav){
    if (*(nav->display_status) & 1){
        *(nav->display_status) &= ~(1);

        uint16_t lat_back_color = BLACK;
        uint16_t lat_txt_color = WHITE;
        uint16_t lon_back_color = BLACK;
        uint16_t lon_txt_color = WHITE;

        if (*(nav->display_status) & (1 << CURSOR_STATE)){
            if(*(nav->display_status) & (1 << LATLON_FOCUS)){
                lon_back_color = WHITE;
                lon_txt_color = BLACK;
            }else{
                lat_back_color = WHITE;
                lat_txt_color = BLACK;
            }
            
        }

        // Print LAT
        if (latIndex(nav->buffer_i) > 0){
            displaySubstr(DEST_LAT_CLMN_START, 0, WHITE, BLACK, 0, latIndex(nav->buffer_i), nav->dest_lat_buffer);
        }

        displayColorChar(latIndex(nav->buffer_i) * 8, 0, lat_txt_color, lat_back_color, nav->dest_lat_buffer[latIndex(nav->buffer_i)]);

        if (latIndex(nav->buffer_i) < LAT_BUFFER_SIZE)
        {
            displaySubstr(0, 0, WHITE, BLACK, latIndex(nav->buffer_i) + 1, LAT_BUFFER_SIZE, nav->dest_lat_buffer);
        }

        displayStr(LAT_BUFFER_SIZE * 8, 0, WHITE, ",");

        // Print LON
        if (lonIndex(nav->buffer_i) > 0){
            displaySubstr(DEST_LON_CLMN_START, 0, WHITE, BLACK, 0, lonIndex(nav->buffer_i), nav->dest_lon_buffer);
        }

        displayColorChar((DEST_LON_CLMN_START + (lonIndex(nav->buffer_i) * 8)), 0, lon_txt_color, lon_back_color, nav->dest_lon_buffer[lonIndex(nav->buffer_i)]);

        if (lonIndex(nav->buffer_i) < LON_BUFFER_SIZE){
            displaySubstr(DEST_LON_CLMN_START, 0, WHITE, BLACK, lonIndex(nav->buffer_i) + 1, LON_BUFFER_SIZE, nav->dest_lon_buffer);
        }
        return;
    }
}

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

uint8_t increment_relevant_index(Nav *nav){
    // is it for lat buffer or lon buffer?
    printString("Display status");
    printBinaryByte(*(nav->display_status));
    printString("\n");
    uint8_t result = 0;
    if (!(*(nav->display_status) & (1 << LATLON_FOCUS)))
    {
        printString("Buffer index\n");
        printBinaryByte(nav->buffer_i);
        printString("\n");
        result = latIndex(nav->buffer_i) + 1; 
        nav->buffer_i = setLatIndex(nav->buffer_i, result);
        printBinaryByte(nav->buffer_i);
    }else{
        // relevant_index = lonIndex(nav->buffer_i);
        // active_buffer = nav->dest_lon_buffer;
        result = lonIndex(nav->buffer_i) + 1;
        nav->buffer_i = setLonIndex(nav->buffer_i, result);
    }
    return result;
}

uint8_t latIndex(uint8_t buffer_i){
    uint8_t result = buffer_i >> 4;
    printString("latitude index: ");
    printBinaryByte(result);
    printString("\n");
    return result;
}

uint8_t setLatIndex(uint8_t buffer_i, uint8_t lat_index){
    printString("setting latitude index to: ");
    printBinaryByte(lat_index);
    printString("\n");
    return (buffer_i & 0xF) | ((lat_index & 0xF) << 4);
}
uint8_t lonIndex(uint8_t buffer_i){
    uint8_t result = buffer_i & 0xF;
    printString("longitude index: ");
    printBinaryByte(result);
    printString("\n");
    return result;
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