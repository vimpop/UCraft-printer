#ifndef _CONFIG__ADV_H
#define _CONFIG__ADV_H

#define ENDIAN 0 // 1: little, 0:big

#define TICK_TIME_MS 5
#define MEM_CHUNK_SIZE 256    // Allocation granularity
#define MEM_CHUNK_THRESHOLD 2 // Number of chunks to allocate before reallocating

#define MAX_SEND_FRAGMENT_SIZE 2048 // Number of bytes to send on the wire each call
#define MAX_STRING_SIZE 1024
#define LOG_BUFFER_SIZE 1024
#define READBUFSIZE 4096

#define AUTH_HOST "sessionserver.mojang.com"
#define AUTH_HOST_PORT "443"
#define AUTH_TIMEOUT 800 // Number of ticks before timing out

#define PLAYER_BASE 0x4

#endif