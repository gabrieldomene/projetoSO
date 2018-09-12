#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>




int main(){

    //criar socket -> bind -> listen -< connection
    int socket_name = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    time_t ticks;

    socket_name = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_name == 0)
    {
        printf("Failed to create socket, exiting. . . %i\n", socket_name);
    }else{
        printf("Server socket created!\n");
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(socket_name, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(socket_name, 10);

    printf("Esperando msg");
    while (1)
    {

        connfd = accept(socket_name, (struct sockaddr*)NULL, NULL);
        sleep(1);
        close(connfd);
    }

    return 0;
}