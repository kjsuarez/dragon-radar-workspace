# This makefile targets atmega168 flashed with a usbasp v3.2
F_CPU = 8000000UL

LOCATION = ../
NAME = display-practice
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
	mkdir -p $(NAME)/src
	touch $(NAME)/src/main.c
	mkdir -p $(NAME)/includes
	touch $(NAME)/includes/main.h
	mkdir -p $(NAME)/build
	cp makefile $(NAME)/makefile
size: build/$(NAME).elf
	avr-size --mcu=$(MCU) -C build/$(NAME).elf
.PHONY: flash, clean, uart, scaffold, size