#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

//===== RomuMono32 ===============================================================================
//
// 32-bit arithmetic: Suitable only up to 2^26 output-values. Outputs 16-bit numbers.
// Fixed period of (2^32)-47. Must be seeded using the romuMono32_init function.
// Capacity = 2^27 bytes. Register pressure = 2. State size = 32 bits.

#define ROTL(x, k) (((x) << (k)) | ((x) >> (32 - (k))))

uint16_t romuMono32_random()
{
    static uint32_t state = (67676767 & 0x1fffffffu) + 1156979152u;
    uint16_t result = state >> 16;
    state *= 3611795771u;
    state = ROTL(state, 12);
    return result;
}

int mbedtls_hardware_poll(void *data,
                          unsigned char *output, size_t len, size_t *olen)
{
    (void)data; // data is unused, may be NULL

    size_t bytes_written = 0;
    while (bytes_written + sizeof(int) <= len)
    {
        int rnd = romuMono32_random();
        memcpy(output + bytes_written, &rnd, sizeof(int));
        bytes_written += sizeof(int);
    }

    if (bytes_written < len)
    {
        int rnd = romuMono32_random();
        memcpy(output + bytes_written, &rnd, len - bytes_written);
        bytes_written = len;
    }

    *olen = bytes_written;
    return 0;
}