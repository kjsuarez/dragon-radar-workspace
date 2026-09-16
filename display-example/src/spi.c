#include "spi.h"

void tradeByte(uint8_t byte){
    SPDR = byte;
    loop_until_bit_is_set(SPSR, SPIF);
}