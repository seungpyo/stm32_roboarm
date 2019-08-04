OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH("arm")

ENTRY(start)

PHDRS {
    load PT_LOAD;
}

MEMORY {
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
    RAM (xrw)   : ORIGIN = 0x20000000, LENGTH = 320K
    CCMRAM (rw) : ORIGIN = 0x10000000, LENGTH = 64K
}

SECTIONS {
    . = 0;
    
    . = ALIGN(4K);
    boot_start = .;

    . = ALIGN(8K);
    .boot . : {
        *(.boot.tables);
        *(.boot);
    } : load
    
    . = ALIGN(16K);
    boot_text_start = .;

    .text : {
        *(.boot.o);
        /* . = ALIGN(4K); */
        *(.text);
        *(.text.unlikely);
    }>FLASH

    .rodata . : {
        *(.rodata);
        *(.rodata.*);
    }

    .note.gnu.build-id . : {
        *(.note.gnu.build-id);
    }

    boot_text_end = .;

    .got . : {
        got_base = .;
        *(.got);
        *(.got.plt);
    }

    .data . : {
        *(.data);
        *(.data.*);
    }

    .bss . : {
        *(.bss);
    }

    .rel.dyn . : {
        *(.rel.dyn);
    }
    
    boot_end = .;

    /DISCARD/ : {
        *(.eh_frame);
        *(.eh_frame_hdr);
        *(.interp);
        *(.dynamic);
    }

}
