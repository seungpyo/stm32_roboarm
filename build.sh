rm -f *.o *.elf *.bin
arm-none-eabi-as -mthumb -o boot.o boot.S 
arm-none-eabi-gcc -c -mthumb main.c
arm-none-eabi-ld -T"linker.ld" -o main.elf boot.o main.o
arm-none-eabi-objcopy -O binary main.elf main.bin

