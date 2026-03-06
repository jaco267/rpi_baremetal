ENTRY(_start)

SECTIONS
{
    .text :
    {
        *(.text.startup)  /* startup code */
        *(.text*)         /* all other text */
    }
    .rodata : { *(.rodata) }
    .data : { *(.data*) }
    . = ALIGN(0x8);   /* pad to 8 bit for bss cleaning */
    bss_begin = .;
    .bss : { *(.bss*) }
    bss_end = .;
}