#include "platform/stubs.h"
#include "platform/sockets/sockets.h"
#include <stdint.h>

uint32_t recv_full(int sock, uint8_t *buffer, uint32_t length)
{
    uint32_t received = 0;
    int res = 0;
    while (length > received)
    {
        res = socket_recv(sock, &buffer[received], length - received, 0);
        if (res <= 0)
        {
            return 0;
        }
        received += (uint32_t)res;
    }
    return received;
}

void entry()
{

    int server_socket = socket_create(PF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        assert("create socket", __FILE_NAME__, __LINE__);
        return;
    }
    sockaddr_in server_addr;
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = 25564;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (socket_bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        assert("bind socket", __FILE_NAME__, __LINE__);
        return;
    }
    if (socket_listen(server_socket, 1) < 0)
    {
        assert("listen on socket", __FILE_NAME__, __LINE__);
        return;
    }
    sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);
    int client_socket = socket_accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen);
    if (client_socket < 0)
    {
        assert("accept socket", __FILE_NAME__, __LINE__);
        return;
    }
    uint32_t size = 0;
    uint32_t received = 0;
    recv_full(client_socket, (uint8_t *)&size, sizeof(size));
    if (size > 0)
    {
        uint8_t *buffer = (uint8_t *)_AllocateMemory(0x45, size, 0);
        if (buffer == NULL)
        {
            assert("allocate memory", __FILE_NAME__, __LINE__);
            return;
        }
        received = recv_full(client_socket, buffer, size);
        if (received != size)
        {
            assert("receive data", __FILE_NAME__, __LINE__);
            return;
        }
        void (*func)() = (void (*)())buffer;
        socket_shutdown(client_socket, SHUT_RDWR);
        socket_shutdown(server_socket, SHUT_RDWR);
        close(client_socket);
        close(server_socket);
        _printf("EP: 0x%x \r\n", func);
        int ret = CreateTask("task", func, NULL, 0x1000, 0x15);
        if(ret != 0){
            assert("create task", __FILE_NAME__, __LINE__);
            return;
        }
        while(1){
            yield_task(100);
        }
        return;
    }
    socket_shutdown(client_socket, SHUT_RDWR);
    socket_shutdown(server_socket, SHUT_RDWR);
    close(client_socket);
    close(server_socket);
    assert("no data received", __FILE_NAME__, __LINE__);
    return;
}
