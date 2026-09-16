#include "st-7789.h"
#include <util/delay.h>
#include "USART.h"

#include "dictionary.h"

// Rules
// ChipSelect is high when idle, low when active
// Data/command pin is low for command, high for data

// initialize the SPI peripheral
void initSpi(void){
    // set MOSI, SCK, and CS as output
    SPI_DDR |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_CS);

    // set CS high (idle)
    SPI_CS_PORT |= (1 << SPI_CS);

    // set MISO pull-up
    SPI_MISO_PORT |= (1 << SPI_MISO);
    
    // enable SPI in master mode, set clock rate
    SPCR |= (1 << SPE) //enable SPI
         | (1 << MSTR); //master mode
    //   | (1 << SPR0); // Since our clock speed is already a pathetic 1MHz a default SCK of fosc/4 is already slow enough, so we don't need to set any clock rate bits
}

uint8_t st7789_command(uint8_t command){
    // set DC low for command
    COMMAND_MODE_SET;
    // select the ST7789
    CHIP_SELECT;
    tradeByte(command);
    CHIP_DESELECT;
    return (SPDR);
}

uint8_t st7789_data(uint8_t data){
    // set DC high for data
    DATA_MODE_SET;
    // select the ST7789
    CHIP_SELECT;
    tradeByte(data);
    CHIP_DESELECT;
    return (SPDR);
}

void clearDisplay(void){
    st7789_command(CASET);
    st7789_data(0x00); // start word
    st7789_data(0x00);
    st7789_data(LAST_ROW_ADDRESS >> 8); // end word
    st7789_data(LAST_ROW_ADDRESS & 0xFF);
    _delay_ms(10);

    st7789_command(RASET);
    st7789_data(0x00); // start word
    st7789_data(0x00);
    st7789_data(LAST_ROW_ADDRESS >> 8); // end word
    st7789_data(LAST_ROW_ADDRESS & 0xFF);
    _delay_ms(10);

    st7789_command(RAMWR);
    for (uint32_t i = 0; i < 239UL * 322UL; i++)
    {
        st7789_data(BLACK);
        st7789_data(BLACK);
    }
}

void initDisplay(void){
    st7789_command(SWRESET);
    _delay_ms(121);

    st7789_command(SLPOUT);
    _delay_ms(10);

    st7789_command(MADCTL);
    st7789_data(0xA0);
    _delay_ms(10);

    st7789_command(COLMOD);
    st7789_data(0x55);
    _delay_ms(10);

    clearDisplay();

    st7789_command(DISPON);
    _delay_ms(10);
}

void firstChar(void){
    st7789_command(CASET);
    st7789_data(0x00); // start word
    st7789_data(0x00);

    st7789_data(0x00); // end word
    st7789_data(0x08);
    _delay_ms(10);

    st7789_command(RASET);
    st7789_data(0x00); // start word
    st7789_data(0x00);
    st7789_data(0x00); // end word
    st7789_data(0x08);
    _delay_ms(10);

    st7789_command(RAMWR);
    for (uint32_t i = 0; i < (8 * 8)-1; i++)
    {
        st7789_data(RED >> 8);
        st7789_data(RED & 0xFF);
    }
}

void displayChar(uint16_t x, uint16_t y, uint16_t color, char character){
    char char_ary[8];
    lookup(character, char_ary);

    st7789_command(CASET);
    st7789_data(x >> 8); // start word
    st7789_data(x & 0xFF);

    st7789_data((x + 0x7) >> 8); // end word
    st7789_data((x + 0x7) & 0xFF);

    st7789_command(RASET);
    st7789_data(y >> 8); // start word
    st7789_data(y & 0xFF);

    st7789_data((y + 0x7) >> 8); // end word
    st7789_data((y + 0x7) & 0xFF);

    st7789_command(RAMWR);
    for (uint8_t i = 0; i < 8; i++){
        for (uint8_t j = 8; j > 0; j--){
            if (char_ary[i] & (1 << (j - 1)))
            {
                st7789_data(color >> 8);
                st7789_data(color & 0xFF);
            }
            else
            {
                // black pixel for zeros
                st7789_data(BLACK);
                st7789_data(BLACK);
            }
        }
    }
}

void displayStr(uint16_t x, uint16_t y, uint16_t color, char str[]){
    for (uint16_t i = 0; str[i] != '\0'; i++){
        displayChar(x + (i * 8), y, color, str[i]);
    }
}