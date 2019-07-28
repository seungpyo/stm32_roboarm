#Makefile

CC = avr-gcc
LD = avr-ld
CFLAGS = -Wall -std=gnu99 -Os -mmcu=atmega128 -DF_CPU=16000000UL
LDFLAGS = -A atmega128 -m avr51

HEX_FLASH = -R .eeprom -R .fuse -R .lock -R .signature -j .text -j .data
FLASH_FLAGS = -p m128 -P /dev/ttyUSB0 -c STK500 -v -v -v -v -e
OBJECT = test.o
TARGET = test.elf

%.o: %.c ;$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJECT)
	$(LD) $(LDFLAGS) $(OBJECT) -o $(TARGET)

%.hex: %.elf ;avr-objcopy -O ihex $(HEX_FLASH) $< $@

flash: test.hex 
	sudo avrdude $(FLASH_FLAGS) -b 115200 -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m -U lock:w:0xFF:m -U flash:w:$<:i

erase: 
	sudo avrdude $(FLASH_FLAGS) -b 115200 -e
clean: 
	$(RM) test.o test.elf test.hex



