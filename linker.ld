ENTRY(reset_handler)

MEMORY
{
    FLASH(RX) : ORIGIN = 0x0, LENGTH = 0x2000           /* 4kb */
    SRAM(RW)  : ORIGIN = 0x20000000, LENGTH = 0x1000    /* 2kb */
}

SECTIONS
{
    .text : /* read only */
    {
        *(.isr_vector*)
        *(.text*)
    } > FLASH

    .data : /* rw allowed */
    {
        *(.data*)
    } > SRAM
}
