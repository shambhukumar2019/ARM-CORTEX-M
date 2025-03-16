ENTRY(reset_handler)

MEMORY
{
    FLASH : ORIGIN = 0x0, LENGTH = 0x1000
}

SECTIONS
{
    .text :
    {
        *(.isr_vector*)
        *(.text*)
    } > FLASH
}