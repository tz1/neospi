# Makefile to build light_ws2812 library examples
# This is not a very good example of a makefile - the dependencies do not work, therefore everything is rebuilt every time.

# Change these parameters for your device

F_CPU = 8000000
DEVICE = attiny85

PRGTYPE=usbtiny
AVRTYPESHORT=t85
# Tools:
CC = avr-gcc

LIB       = light_ws2812

CFLAGS = -I. -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -Os -Wall

LDFLAGS = -Wl,--relax,--section-start=.text=0,-Map=main.map

all:	neospi.hex

neospi.hex: neospi.c
	$(CC) $(CFLAGS) -S -o neospi.S neospi.c
	$(CC) $(CFLAGS) -o neospi.o neospi.c
	avr-size neospi.o
	avr-objcopy -j .text -j .data -O ihex neospi.o neospi.hex

flash: neospi.hex
	avrdude -c ${PRGTYPE} -p ${AVRTYPESHORT} -U flash:w:neospi.hex

clean:
	rm -f *.o *.hex *.S
