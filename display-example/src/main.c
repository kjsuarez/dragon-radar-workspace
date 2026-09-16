#include <avr/io.h>
#include <util/delay.h>
#include "st-7789.h"
#include "USART.h"

int main(void){
    // initialize the Data/Control pin and Chip Select pin as outputs
    SPI_DDR |= (1 << DISPLAY_DC);

    // initialize the display
    initSpi();
    
    initUSART();
    initDisplay();
    displayStr(0, 0, WHITE, "U R ");
    displayStr(28, 0, GREEN, "TOAD");
    displayStr(60, 0, WHITE, "ALLY GAY");
    while (1){
        transmitByte('x');
        transmitByte('\n');
    }
    return 0;
}