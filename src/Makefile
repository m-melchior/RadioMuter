###############################################################################
# Makefile for the project RadioMuter
###############################################################################

## General Flags
PROJECT = RadioMuter
MCU = at90can128
TARGET = ${PROJECT}.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall
CFLAGS += -gdwarf-2
CFLAGS += -Os
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -MD
CFLAGS += -MP
CFLAGS += -MT $(*F).o
CFLAGS += -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=${PROJECT}.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"../src"
INCLUDES += -I"../libraries/lib_mcu"
INCLUDES += -I"../libraries/lib_mcu/can"

## Objects that must be built in order to link
OBJECTS = main.o
OBJECTS += can_drv.o
OBJECTS += can_lib.o

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) ${PROJECT}.hex ${PROJECT}.eep ${PROJECT}.lss size
burn: all burn

## Compile
main.o: ../src/main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

can_drv.o: ../libraries/lib_mcu/can/can_drv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

can_lib.o: ../libraries/lib_mcu/can/can_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

burn: ${TARGET}
	avrdude -p c128 -c stk500v2 -D -P COM9 -V -u -v -U flash:w:${PROJECT}.hex

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) ${PROJECT}.elf dep/* ${PROJECT}.hex ${PROJECT}.eep ${PROJECT}.lss ${PROJECT}.map

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

