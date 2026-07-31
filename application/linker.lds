OUTPUT_FORMAT("elf32-bigarm")
OUTPUT_ARCH(arm)
GROUP(
   libgcc.a
   libc.a
   libm.a
   libnosys.a
)
ENTRY(entry)

/*
 * ORIGIN below is a link-time placeholder, not the real load address.
 */
MEMORY
{
    RAM (rwx) : ORIGIN = 0x00000000, LENGTH = 0x00200000
}

__heap_size__ = 0x90000;   

SECTIONS
{
    .text : ALIGN(4)
    {
        KEEP(*(.text.entry))
        *(.text*)
        *(.rodata*)
    } > RAM

    .rel.dyn : ALIGN(4)
    {
        *(.rel.dyn)
    } > RAM
    __rel_start__ = ADDR(.rel.dyn);
    __rel_end__   = ADDR(.rel.dyn) + SIZEOF(.rel.dyn);

    .got : ALIGN(4)
    {
        __got_start__ = .;         
        *(.got)
        *(.got.plt)
        *(.igot)                   
        *(.got.*)                  
        . = ALIGN(4);
        __got_end__ = .;          
    } > RAM 

    .data : ALIGN(4)
    {
        *(.data*)
    } > RAM
    
    .tdata : ALIGN(4)
    { 
        __tdata_start = .;
        *(.tdata*)
        *(.gnu.linkonce.td.*)       
        . = ALIGN(4);
        __tdata_end = .;
    } > RAM
    __tdata_size   = SIZEOF(.tdata);
    .tbss : ALIGN(4)
    {
        __tbss_start = .;
        *(.tbss*)
        *(.gnu.linkonce.tb.*)
        *(.tcommon)
        . = ALIGN(4);
        __tbss_end = .;
    } > RAM
    .bss : ALIGN(4)
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM


    __image_end__ = .;

    .heap : ALIGN(8)
    {
        __heap_start__ = .;
        . = . + __heap_size__;
        __heap_end__ = .;
    } > RAM

    __stack_top__ = ORIGIN(RAM) + LENGTH(RAM);

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
