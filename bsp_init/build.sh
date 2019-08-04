rm -f *.o *.elf *.bin
arm-none-eabi-as -mthumb -o boot.o boot.S 
arm-none-eabi-gcc -g -ggdb3 -c -mthumb bsp_init.c init.c led.c usart.c irq.c
arm-none-eabi-ld -T"linker.ld" -o main.elf boot.o bsp_init.o init.o led.o usart.o irq.o
arm-none-eabi-objcopy -O binary main.elf main.bin

