#ifndef SELECT_H
#define SELECT_H
#include <stdint.h>
#include <string.h>

typedef struct fd_set
{
	uint32_t fds_bits[64];
} fd_set;

static inline int FD_ISSET(unsigned long n, struct fd_set *p)
{
	uint32_t mask = (uint32_t)1 << (n % 32);
	return (p->fds_bits[n / 32] & mask) != 0;
}

static inline void FD_SET(unsigned long n, struct fd_set *p)
{
	uint32_t mask = (uint32_t)1 << (n % 32);
	p->fds_bits[n / 32] |= mask;
}

static inline void FD_CLR(unsigned long n, struct fd_set *p)
{
	uint32_t mask = (uint32_t)1 << (n % 32);
	p->fds_bits[n / 32] &= ~mask;
}

static inline void FD_ZERO(struct fd_set *p)
{
	memset(p, 0, sizeof(*p));
}

#endif /* SELECT_H */