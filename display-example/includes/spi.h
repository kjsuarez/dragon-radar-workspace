#include <avr/io.h>

extern void tradeByte(uint8_t byte);

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_PIN PINB

#define SPI_MISO PB4
#define SPI_MISO_DDR DDRB
#define SPI_MISO_PORT PORTB
#define SPI_MISO_PIN PINB

#define SPI_MOSI PB3
#define SPI_MOSI_DDR DDRB
#define SPI_MOSI_PORT PORTB
#define SPI_MOSI_PIN PINB

#define SPI_SCK PB5
#define SPI_SCK_DDR DDRB
#define SPI_SCK_PORT PORTB
#define SPI_SCK_PIN PINB

#define SPI_CS PB2
#define SPI_CS_DDR DDRB
#define SPI_CS_PORT PORTB
#define SPI_CS_PIN PINB