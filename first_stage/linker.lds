OUTPUT_FORMAT("elf32-bigarm")
OUTPUT_ARCH(arm)
ENTRY(entry)


SECTIONS
{
    .text : ALIGN(4)
    {
        KEEP(*(.text.entry))
        *(.text*)
        *(.rodata*)
    }

    /DISCARD/ :
    {
        *(.dynamic)
        *(.hash)
        *(.dynsym)
        *(.dynstr)
        *(.interp)
        *(.comment)
        *(.note*)
    }
}
