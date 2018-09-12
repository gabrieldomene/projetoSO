#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>

int main()
{
    int socket_name = 0, connfd = 0;
    int c;
    struct sockaddr_in serv_addr;

    socket_name = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_name == 0)
    {
        printf("Failed to create socket, exiting. . .\n");
    }else{
        printf("Client socket created!\n");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(5000);

    //bind(socket_name, (struct sockadder*)&serv_adder, sizeof(serv_addr));
    //listen(socket_name, 10);
    connfd = connect(socket_name, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in));
    if (connfd < 0)
    {
        printf("connfd %d: \n", connfd);
        fprintf(stderr, "Error");
        return 1;
    }
    
    return 0;
}