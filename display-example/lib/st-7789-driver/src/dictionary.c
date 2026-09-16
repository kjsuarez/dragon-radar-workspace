#include <avr/pgmspace.h>

void lookup(char character, char *output)
{
    static const char qmark_char[] PROGMEM = {0x1c, 0x22, 0x22, 0x02, 0x04, 0x08, 0x00, 0x08};
    static const char A_char[] PROGMEM = {0x18, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42};
    static const char U_char[] PROGMEM = {0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1c};
    static const char R_char[] PROGMEM = {0x3c, 0x42, 0x42, 0x42, 0x7c, 0x44, 0x42, 0x42};
    static const char T_char[] PROGMEM = {0x7f, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08};
    static const char O_char[] PROGMEM = {0x1c, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1c};
    static const char D_char[] PROGMEM = {0x7c, 0x42, 0x41, 0x41, 0x41, 0x41, 0x42, 0x7c};
    static const char L_char[] PROGMEM = {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7c};
    static const char Y_char[] PROGMEM = {0x22, 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08};
    static const char G_char[] PROGMEM = {0x3c, 0x42, 0x42, 0x40, 0x40, 0x4e, 0x42, 0x3c};
    static const char space_char[] PROGMEM = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    const char *placeholder;

    switch (character)
    {
    case 'A':
        placeholder = A_char;
        break;
    case 'U':
        placeholder = U_char;
        break;
    case 'R':
        placeholder = R_char;
        break;
    case 'T':
        placeholder = T_char;
        break;
    case 'O':
        placeholder = O_char;
        break;
    case 'D':
        placeholder = D_char;
        break;
    case 'L':
        placeholder = L_char;
        break;
    case 'Y':
        placeholder = Y_char;
        break;
    case 'G':
        placeholder = G_char;
        break;
    case ' ':
        placeholder = space_char;
        break;
    default:
        placeholder = qmark_char;
        break;
    }

    for (uint8_t i = 0; i < 8; i++)
    {
        output[i] = pgm_read_byte(&placeholder[i]);
    }
}
