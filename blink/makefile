F_CPU = 1000000UL
NAME = blink
# PROGRAMMER = avrisp
PROGRAMMER = usbasp
PORT = /dev/ttyUSB0
# PORT = usb



flash: build/$(NAME).hex
	avrdude -p m168 -c $(PROGRAMMER) -P /dev/ttyUSB0 -b 19200 -U flash:w:build/$(NAME).hex
build/$(NAME).hex: build/$(NAME).elf
	avr-objcopy -j .text -j .data -O ihex build/$(NAME).elf build/$(NAME).hex
size: build/$(NAME).elf
	avr-size --mcu=$(MCU) -C build/$(NAME).elf
build/$(NAME).elf: src/main.c
	avr-gcc -mmcu=atmega168 -Wall -Os -o build/$(NAME).elf src/main.c
clean:
	rm -f build/$(NAME).elf build/$(NAME).hex
uart:
	minicom -D /dev/ttyUSB0 -b 9600

.PHONY:  clean, uart, scaffold, size
