F_CPU = 1000000UL
NAME = numpad
PROGRAMMER = usbasp
PORT = /dev/ttyUSB0


CC_FLAGS = -I./src
CC_FLAGS += -I./includes

flash: build/$(NAME).hex
	avrdude -p m168 -c $(PROGRAMMER) -P /dev/ttyUSB0 -b 19200 -U flash:w:build/$(NAME).hex
build/$(NAME).hex: build/$(NAME).elf
	avr-objcopy -j .text -j .data -O ihex build/$(NAME).elf build/$(NAME).hex
build/$(NAME).elf: src/main.c
	avr-gcc -mmcu=atmega168 -Wall -Os -o build/$(NAME).elf src/main.c src/numpad.c src/USART.c -DF_CPU=$(F_CPU) $(CC_FLAGS)
clean:
	rm -f build/$(NAME).elf build/$(NAME).hex
uart:
	minicom -D /dev/ttyUSB0 -b 9600
.PHONY: flash, clean, uart