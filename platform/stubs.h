#ifndef STUBS_H
#define STUBS_H
#include <stdint.h>
#include <stddef.h>
#include "platform/sockets/sockets.h"
#include "platform/sockets/select.h"
#include "platform/time.h"
// System & Utility Templates
typedef void ledshow_t(void);
typedef void __uart_putc_t(int module, char c);
typedef int close_t(int fd);
typedef void assert_t(const char *expr, const char *file, int line);
typedef void get_time_t(struct timeval *t);
typedef void yield_task_t(int delay); // 100 == 1000ms
typedef int ioctl_t(int __fd, uint32_t __request, ...);
typedef void _printf_t(const char *format, ...);
typedef int CreateTask_t(const char *taskname, void *task, void *param, int stacksize, int priority);

// Memory Management Templates
typedef void *_AllocateMemory_t(int param1, int size, int param3);
typedef void *_FreeMemory_t(int param1, void *ptr, int param3);

// Networking / Socket Templates
typedef int socket_create_t(int domain, int type, int protocol);
typedef int socket_bind_t(int fd, const struct sockaddr *addr, socklen_t addrlen);
typedef int socket_listen_t(int fd, int backlog);
typedef int socket_accept_t(int fd, struct sockaddr *addr, socklen_t *addrlen);
typedef int socket_connect_t(int fd, const struct sockaddr *addr, socklen_t addrlen);
typedef int socket_recv_t(int fd, void *buf, size_t len, int flags);
typedef int socket_send_t(int fd, const void *buf, size_t len, int flags);
typedef int socket_getpeername_t(int fd, struct sockaddr *addr, socklen_t *addrlen);
typedef int socket_select_t(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
typedef int socket_shutdown_t(int fd, int flag);
typedef int socket_setsockopt_t(int fd, int level, int optname, const void *optval, socklen_t optlen);
typedef int socket_getaddrinfo_t(const char *name, const char *service, const struct addrinfo *req, struct addrinfo **pai);
typedef int socket_freeaddrinfo_t(struct addrinfo *ai);
typedef void* socket_gethostbyname_t(const char *name);


typedef char *inet_ntoa_t(struct in_addr in, const char *buffer);

#define ABS_ADDR(addr) ((addr) & ~3)

// System & Utility Macros
#define led_show ((ledshow_t *)ABS_ADDR(0x40735054))
#define __uart_putc ((__uart_putc_t *)ABS_ADDR(0x40144304))
#define close ((close_t *)ABS_ADDR(0x40504a68))
#define assert ((assert_t *)ABS_ADDR(0x404fcf8c))
#define get_time ((get_time_t *)ABS_ADDR(0x405bb36c))
#define yield_task ((yield_task_t *)ABS_ADDR(0x404fde78))
#define ioctl ((ioctl_t *)ABS_ADDR(0x40504b14))
#define _printf ((_printf_t *)ABS_ADDR(0x4013cf40))
#define CreateTask ((CreateTask_t *)ABS_ADDR(0x404fd634))

// Memory Management Macros
#define _AllocateMemory ((_AllocateMemory_t *)ABS_ADDR(0x40508e7c))
#define _FreeMemory ((_FreeMemory_t *)ABS_ADDR(0x40509048))

// Networking / Socket Macros
#define socket_create ((socket_create_t *)ABS_ADDR(0x40504c9c))
#define socket_bind ((socket_bind_t *)ABS_ADDR(0x40504a4c))
#define socket_listen ((socket_listen_t *)ABS_ADDR(0x40504b50))
#define socket_accept ((socket_accept_t *)ABS_ADDR(0x40504a30))
#define socket_connect ((socket_connect_t *)ABS_ADDR(0x40504a94))
#define socket_recv ((socket_recv_t *)ABS_ADDR(0x40504b6c))
#define socket_send ((socket_send_t *)ABS_ADDR(0x40504c00))
#define socket_shutdown ((socket_shutdown_t *)ABS_ADDR(0x40504c80))
#define socket_setsockopt ((socket_setsockopt_t *)ABS_ADDR(0x40504c54))
#define socket_getpeername ((socket_getpeername_t *)ABS_ADDR(0x40504ab0))
#define socket_select ((socket_select_t *)ABS_ADDR(0x40504bc0))
#define socket_getaddrinfo ((socket_getaddrinfo_t *)ABS_ADDR(0x405be844))
#define socket_freeaddrinfo ((socket_freeaddrinfo_t *)ABS_ADDR(0x405be878))
#define socket_gethostbyname ((socket_gethostbyname_t *)ABS_ADDR(0x405be5c0)) //0x40617f24

#define inet_ntoa ((inet_ntoa_t *)ABS_ADDR(0x40504dd0))
#endif // STUBS_H