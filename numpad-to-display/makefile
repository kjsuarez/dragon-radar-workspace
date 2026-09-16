# This makefile targets atmega168 flashed with a usbasp v3.2
F_CPU = 8000000UL

LOCATION = ../
NAME = numpad-to-display
PROJECT = $(LOCATION)$(NAME)

PROGRAMMER = usbasp
PORT = /dev/ttyUSB0
MCU = atmega168

SRC_PATH = src/
SRC_FILES = $(wildcard $(SRC_PATH)*.c)

LIBRARIES = $(wildcard lib/*)
LIBRARIES_SRC = $(wildcard lib/*/src/*.c)
LIBRARIES_INCLUDES = $(sort $(dir $(wildcard lib/*/includes/*.h)))
CC_FLAGS = -I./src
CC_FLAGS += -I./includes
CC_FLAGS += $(addprefix -I, $(LIBRARIES_INCLUDES))

MATH_LIB = -lm

build/$(NAME).hex: build/$(NAME).elf
	avr-objcopy -j .text -j .data -O ihex build/$(NAME).elf build/$(NAME).hex
build/$(NAME).elf: $(SRC_FILES) $(LIBRARIES_SRC) $(wildcard includes/*)
	avr-gcc -mmcu=atmega168 -Wall -Os -o build/$(NAME).elf $(SRC_FILES) $(LIBRARIES_SRC) -DF_CPU=$(F_CPU) $(CC_FLAGS)
clean:
	rm -f build/$(NAME).elf build/$(NAME).hex
flash:
	avrdude -p m168 -c $(PROGRAMMER) -U lfuse:w:0xE2:m -P /dev/ttyUSB0 -b 19200 -U flash:w:build/$(NAME).hex
uart:
	minicom -D /dev/ttyUSB0 -b 9600
scaffold:
	mkdir -p $(PROJECT)
	mkdir -p $(PROJECT)/src
	cp main.c $(PROJECT)/src/main.c
	mkdir -p $(PROJECT)/includes
	touch $(PROJECT)/includes/main.h
	mkdir -p $(PROJECT)/lib
	mkdir -p $(PROJECT)/build
	mkdir -p $(PROJECT)/tests
	mkdir -p $(PROJECT)/tests/build
	cp -r unity $(PROJECT)/tests/unity
	cp makefile $(PROJECT)/makefile
	cp test.mk $(PROJECT)/test.mk
size: build/$(NAME).elf
	avr-size --mcu=$(MCU) -C build/$(NAME).elf
.PHONY: flash, clean, uart, scaffold, size

include test.mk