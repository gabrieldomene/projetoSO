#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>

int main()
{
    //criar socket -> bind -> listen -> connection
    int socket_name = 0, new_conn = 0;
    struct sockaddr_in serv_addr;
    char msg_recv[20];

    char sendBuff[1025];

    socket_name = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_name < 0)
    {
        fprintf(stderr, "Failed to create socket, exiting");
    }else{

        fprintf(stderr, "Success");
        
    }
    
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);
    bind(socket_name, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(socket_name, 10);
    
    while (1)
    {   
        new_conn = accept(socket_name, (struct sockaddr*)NULL, NULL);
        read(new_conn, msg_recv, sizeof(msg_recv));
        fprintf(stderr, "New: %s", msg_recv);
        close(new_conn);
     }   
    
    return 0;
}