#ifndef __ASSEMBLER__
#define __ASSEMBLER__ 1


    .globl start
    
    .bss
    .align 4

boot_stack:
    .space 0x4000, 0
boot_stack_top:

    .section .text
    .arm
    
    // .extern got_base

start:
    // Setting up interrupt vector table
    .word 0x20004000            // 0x0; Initial MSP 
    .word main                  // 0x4; Reset Handler
    .word nmi_handler           // 0x8; NMI handler
    .word hard_fault_handler    // 0xc: hard fault handler
    .word mm_fault_handelr      // 0x10; memory management fault handler
    .word bus_fault_handler     // 0x14; bus fault handler
    .word usage_fault_handler   // 0x18; usage fault handelr
    .space 0x10, 0              // 0x20:0x2b; reserved
                                // 0x2C
    .space 0x4000 - 0x2C, 0

    // Switch to system mode, with aborts and interrupts off.
    cpsid aif
 
reset_handler:
	b main

nmi_handler:
    b nmi_handler

hard_fault_handler:
    b irq_hard_fault_handler

mm_fault_handelr:
    b mm_fault_handelr

bus_fault_handler:
    b bus_fault_handler

usage_fault_handler:
    b usage_fault_handler



#endif /* __ASSEMBLER__ */
