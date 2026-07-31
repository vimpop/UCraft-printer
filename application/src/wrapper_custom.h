#ifndef WRAPPER_CUSTOM_H
#define WRAPPER_CUSTOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform/stubs.h"
#include "platform/sockets/sockets.h"
#include "platform/sockets/select.h"
#include "platform/ioctl.h"
#include <errno.h>

// Endian swapping definitions
#define __bswap_16(x) __bswap16(x)
#define __bswap_32(x) __bswap32(x)
#define __bswap_64(x) __bswap64(x)

// Misc functions
static inline void U_wrapperStart()
{
    return;
}
static inline void U_wrapperEnd()
{
    return;
}
static inline void U_sleep(int msec)
{
    yield_task(msec);
}
static inline uint64_t U_millis()
{
    struct timeval tv;
    get_time(&tv);
    return tv.tv_sec * (uint64_t)1000 + (tv.tv_usec) / 1000;
}
// Networking functions
static inline int U_socket(int domain, int type, int protocol)
{
    return socket_create(domain, type, protocol);
}

static inline int U_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
    return socket_setsockopt(sockfd, level, optname, optval, optlen);
}
static inline int U_setsocknonblock(int sockfd)
{
    int blocking = 1;
    ioctl(sockfd, FIONBIO, &blocking);
    return 1;
}
static inline int U_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return socket_bind(sockfd, addr, addrlen);
}

static inline int U_listen(int sockfd, int backlog)
{
    return socket_listen(sockfd, backlog);
}

static inline int U_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
    return socket_select(nfds, readfds, writefds, exceptfds, timeout);
}

static inline int U_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    return socket_accept(sockfd, addr, addrlen);
}

static inline int U_getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    return socket_getpeername(sockfd, addr, addrlen);
}

static inline int U_getaddrinfo(const char *name, const char *service, const struct addrinfo *req, struct addrinfo **pai)
{
    return socket_getaddrinfo(name, service, req, pai);
}

static inline void U_freeaddrinfo(struct addrinfo *ai)
{
    socket_freeaddrinfo(ai);
    return;
}

static inline struct hostent *U_gethostbyname(const char *name)
{
    static struct in_addr mojang_ip;
    static struct hostent info;
    // bodge solution as i was unable to find this function in the ROM.
    mojang_ip.s_addr =  0x96ab6dc1; // -> 150.171.109.193 -> mr-b01.tm-azurefd.net
    info.h_addr = (struct in_addr *)&mojang_ip.s_addr;
    return &info;
    //TODO: Find the dang stub man
}

static inline char *U_inet_ntoa(struct in_addr in)
{
    // Internal buffer is used
    return inet_ntoa(in, NULL);
}

static inline ssize_t U_recv(int sockfd, void *buf, size_t len, int flags)
{
    ssize_t ret = socket_recv(sockfd, buf, len, flags);
    return ret;
}

static inline ssize_t U_send(int sockfd, const void *buf, size_t len, int flags)
{
    ssize_t ret = socket_send(sockfd, buf, len, flags);
    return ret;
}

static inline int U_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return socket_connect(sockfd, addr, addrlen);
}

static inline int U_close(int fd)
{
    return close(fd);
}

static inline int U_shutdown(int sockfd, int how)
{
    return socket_shutdown(sockfd, how);
}
// Memory function
static inline void *U_malloc(size_t size)
{
    return malloc(size);
}

static inline void *U_calloc(size_t nmemb, size_t size)
{
    return calloc(nmemb, size);
}

static inline void *U_realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}

static inline void U_free(void *ptr)
{
    free(ptr);
}
#endif
