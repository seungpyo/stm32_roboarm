rm -f *.o *.elf *.bin
arm-none-eabi-as -o boot.o boot.S 
arm-none-eabi-gcc -c main.c
arm-none-eabi-ld -T"linker.ld" -o main.elf boot.o main.o
arm-none-eabi-objcopy -O binary main.elf main.bin

