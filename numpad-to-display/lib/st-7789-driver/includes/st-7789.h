#include "spi.h"

#define DISPLAY_DC PB1
#define DISPLAY_DC_DDR DDRB
#define DISPLAY_DC_PORT PORTB

#define COMMAND_MODE_SET DISPLAY_DC_PORT &= ~(1 << DISPLAY_DC)
#define DATA_MODE_SET DISPLAY_DC_PORT |= (1 << DISPLAY_DC)

#define CHIP_SELECT SPI_CS_PORT &= ~(1 << SPI_CS);
#define CHIP_DESELECT SPI_CS_PORT |= (1 << SPI_CS)

// Commands
#define SWRESET 0x01
#define SLPOUT 0x11
#define MADCTL 0x36
#define COLMOD 0x3A
#define DISPON 0x29
#define CASET 0x2A
#define RASET 0x2B
#define RAMWR 0x2C

// pixel addresses
#define LAST_COLUMN_ADDRESS 0xEF
#define LAST_ROW_ADDRESS_START 0x01
#define LAST_ROW_ADDRESS_END 0x3F
#define LAST_ROW_ADDRESS 0x13F


// colors
#define RED 0xf800
#define BLACK 0x00
#define WHITE 0xFFFF
#define GREEN 0x07e0

extern void initSpi(void);
extern uint8_t st7789_command(uint8_t command);
extern uint8_t st7789_data(uint8_t data);
extern void initDisplay(void);
extern void clearDisplay(void);
extern void firstChar(void);
void displayChar(uint16_t x, uint16_t y, uint16_t color, char character);
void displayStr(uint16_t x, uint16_t y, uint16_t color, char str[]);
void displaySubstr(uint16_t x, uint16_t y, uint16_t txt_color, uint16_t bck_color, uint8_t strt_i, uint8_t end_i, char str[]);
void displayColorChar(uint16_t x, uint16_t y, uint16_t txt_color, uint16_t bck_color, char character);
