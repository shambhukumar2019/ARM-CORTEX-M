/* ARM CORTEX-M is LITTLE ENDIAN */

ENTRY(reset_handler)

MEMORY
{
    FLASH(RX) : ORIGIN = 0x0, LENGTH = 0x2000           /* 4kb */
    SRAM(RWX) : ORIGIN = 0x20000000, LENGTH = 0x1000    /* 2kb */
}

SECTIONS
{
    .text : /* rx */
    {
        *(.isr_vector*)
        *(.text*)
    } > FLASH

    .data : /* rwx allowed */
    {
        sstack1 = .;
        *(.data.stack1)
        sstack2 = .;
        *(.data.stack2)
        sstack3 = .;
        *(.data.stack3)
    } > SRAM
}
