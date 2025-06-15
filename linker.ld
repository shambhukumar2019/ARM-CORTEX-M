/* ARM CORTEX-M is LITTLE ENDIAN */

ENTRY(reset_handler)


MEMORY
{
    FLASH(RX) : ORIGIN = 0x08000000, LENGTH = 1024K    /* 1MB */
    SRAM(RWX) : ORIGIN = 0x20000000, LENGTH = 128K     /* 196KB */
    CCRAM(RWX): ORIGIN = 0x10000000, LENGTH = 64K      /* 64KB */ 
}

_min_stack_size = 0x400; /* 400B */
_estack = ORIGIN(SRAM) + _min_stack_size; /* sp decreases when pushed */


SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        /* keep forces do not discard this section */
        KEEP(*(.isr_vector*))
        . = ALIGN(4);
    } > FLASH

    .text : /* rx */
    {
        . = ALIGN(4);
        _stext = .;
        *(.text.reset_handler)
        *(.text*)
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    .data : /* rwx allowed */
    {
        . = ALIGN(8);
        _sdata = .;
        *(.data*);
        . = ALIGN(8); 
        _edata = .; 
    } > SRAM AT > FLASH
    /*
        This tells the linker to place the data section in
        RAM, but the initial values are stored in flash. 
        The startup code must then copy these values from 
        flash to RAM.
    */
    _sdata_flash = LOADADDR(.data);

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*);
        . = ALIGN(4);
        _ebss = .; 
    } > SRAM

}

