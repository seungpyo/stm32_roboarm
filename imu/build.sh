rm -f *.o *.elf *.bin
arm-none-eabi-as -mthumb -o boot.o boot.S 
arm-none-eabi-gcc -g -ggdb3 -c -mthumb main.c led.c usart.c utils.c
arm-none-eabi-ld -T"linker.ld" -o main.elf boot.o main.o led.o usart.o utils.o
arm-none-eabi-objcopy -O binary main.elf main.bin

