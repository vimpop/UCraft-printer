.section .text.entry
.global entry
.type entry, %function
.code 32

.equ R_ARM_RELATIVE, 23

entry:
    adr     r4, entry               @ r4 = runtime load address (relocation delta)

    ldr     r7, =__bss_start__
    add     r7, r7, r4
    ldr     r8, =__bss_end__
    add     r8, r8, r4
    ldr     r9, =__stack_top__
    add     r9, r9, r4

    /*
     * Apply R_ARM_RELATIVE fixups
     */
    ldr     r0, =__rel_start__
    add     r0, r0, r4
    ldr     r1, =__rel_end__
    add     r1, r1, r4
.Lrelocate_loop:
    cmp     r0, r1
    bhs     .Lrelocate_done
    ldr     r2, [r0]                @ r_offset (link-time offset of the fixup target)
    ldr     r3, [r0, #4]            @ r_info
    and     r3, r3, #0xff
    cmp     r3, #R_ARM_RELATIVE
    bne     .Lrelocate_next
    add     r2, r2, r4              @ runtime address of the fixup target
    ldr     r5, [r2]
    add     r5, r5, r4              @ relocate the value it holds
    str     r5, [r2]
.Lrelocate_next:
    add     r0, r0, #8
    b       .Lrelocate_loop
.Lrelocate_done:
    mov     r0, r7
    mov     r1, r8
    mov     r2, #0
.Lbss_clear:
    cmp     r0, r1
    strlo   r2, [r0], #4
    blo     .Lbss_clear

    mov     sp, r9

    bl      app @void app(void)
.Lhang:
    b       .Lhang

.size entry, . - entry
