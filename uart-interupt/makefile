# This makefile targets atmega168 flashed with a usbasp v3.2
F_CPU = 1000000UL

LOCATION = ../
NAME = uart-interupt
PROJECT = $(LOCATION)$(NAME)

PROGRAMMER = usbasp
PORT = /dev/ttyUSB0
MCU = atmega168

SRC_PATH = src/
SRC_FILES = $(wildcard $(SRC_PATH)*.c)

CC_FLAGS = -I./src
CC_FLAGS += -I./includes

MATH_LIB = -lm

build/$(NAME).hex: build/$(NAME).elf
	avr-objcopy -j .text -j .data -O ihex build/$(NAME).elf build/$(NAME).hex
size: build/$(NAME).elf
	avr-size --mcu=$(MCU) -C build/$(NAME).elf
build/$(NAME).elf: $(SRC_FILES) $(wildcard includes/*)
	avr-gcc -mmcu=atmega168 -Wall -Os -o build/$(NAME).elf $(SRC_FILES) -DF_CPU=$(F_CPU) $(CC_FLAGS)
clean:
	rm -f build/$(NAME).elf build/$(NAME).hex
flash:
	avrdude -p m168 -c $(PROGRAMMER) -P /dev/ttyUSB0 -b 19200 -U flash:w:build/$(NAME).hex
uart:
	minicom -D /dev/ttyUSB0 -b 9600
scaffold:
	mkdir -p $(PROJECT)
	mkdir -p $(PROJECT)/src
	touch $(PROJECT)/src/main.c
	mkdir -p $(PROJECT)/includes
	touch $(PROJECT)/includes/main.h
	mkdir -p $(PROJECT)/build
	mkdir -p $(PROJECT)/tests
	mkdir -p $(PROJECT)/tests/build
	cp -r unity $(PROJECT)/tests/unity
	cp makefile $(PROJECT)/makefile
	cp test.mk $(PROJECT)/test.mk
.PHONY: flash, clean, uart, scaffold, size

include test.mk