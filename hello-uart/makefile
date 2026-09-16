F_CPU = 1000000UL
NAME = loopback

CC_FLAGS = -I./src
CC_FLAGS += -I./includes

build/$(NAME).hex: build/$(NAME).elf
	avr-objcopy -j .text -j .data -O ihex build/$(NAME).elf build/$(NAME).hex
build/$(NAME).elf: src/main.c
	avr-gcc -mmcu=atmega168 -Wall -Os -o build/$(NAME).elf src/main.c src/USART.c -DF_CPU=$(F_CPU) $(CC_FLAGS)
clean:
	rm -f build/$(NAME).elf build/$(NAME).hex
flash:
	avrdude -p m168 -c avrisp -P /dev/ttyUSB0 -b 19200 -U flash:w:build/$(NAME).hex
.PHONY: flash