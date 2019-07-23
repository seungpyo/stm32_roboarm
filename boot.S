#ifndef __ASSEMBLER__
#define __ASSEMBLER__ 1


    .globl start
    
    .bss
    .align 8

boot_stack:
    .space 0x4000, 0
boot_stack_top:

    .section .text
    .arm
    
    // .extern got_base

start:
    .word 0x20000000
    .word main  // Reset Handler
    .word 0x0   // Other handler?
    .space 0x4000-0xc, 0

#endif /* __ASSEMBLER__ */
