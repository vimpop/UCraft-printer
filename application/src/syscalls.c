#include <stdint.h>
#include <stdio.h>
#include "platform/stubs.h"

extern char __heap_start__;
extern char __heap_end__;

static char *brk = &__heap_start__;

extern char __tdata_start;

typedef struct tls_index {
    unsigned long ti_module; /* Module ID (1 for main application executable) */
    unsigned long ti_offset; /* Offset of the variable inside the TLS block */
} tls_index_t;

void *__tls_get_addr(tls_index_t *ti) {
    printf("tdata_start: 0x%x offset: 0x%X\r\n ", ti->ti_offset);
    return (void *)(&__tdata_start + ti->ti_offset);
}

void *sbrk(intptr_t incr)
{
    char *old = brk;
    if (incr < 0 || brk + incr > &__heap_end__)
        return (void *)-1;
    brk += incr;
    return old;
}

static int uart_putc(char c, FILE *file)
{
    (void)file;
    __uart_putc(0, c);
    return c;
}

static int uart_getc(FILE *file)
{
    unsigned char c;
    (void)file;
    //	c = __uart_getc();	/* Defined by underlying system */
    return c;
}
static FILE __stdio = FDEV_SETUP_STREAM(uart_putc, uart_getc, NULL, _FDEV_SETUP_RW);

FILE *const stdin = &__stdio;
__strong_reference(stdin, stdout);
__strong_reference(stdin, stderr);