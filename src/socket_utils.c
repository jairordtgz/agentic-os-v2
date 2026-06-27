#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "socket_utils.h"

int conectarServidor()
{
    int socketCliente;

    struct sockaddr_in servidor;

    socketCliente = socket(AF_INET, SOCK_STREAM, 0);

    if(socketCliente < 0)
    {
        perror("socket");
        return -1;
    }

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(SERVER_PORT);
    servidor.sin_addr.s_addr = inet_addr(SERVER_IP);

    if(connect(socketCliente,
               (struct sockaddr *)&servidor,
               sizeof(servidor)) < 0)
    {
        perror("connect");
        close(socketCliente);
        return -1;
    }

    return socketCliente;
}
