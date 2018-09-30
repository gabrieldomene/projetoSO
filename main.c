#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#define N_THREAD 5

int con = 0;
pthread_t threads[N_THREAD];

void *thread_tratadora(void * args)
{
    int k, msg;
    int conn = (int *) args;
    char msg_recv[1024];
    printf("\nConection made\n");
    //Thread roda eterno até sair
    while(1)
    {
        msg = read(conn, msg_recv, sizeof(msg_recv));
        printf("\n[%i] Conn msg: ", conn);
        for (k = 0; k < msg; k++){
            printf("%c",msg_recv[k]); 
        }
    }
    /* pthread_exit(0); */
}

void socket_creation();

int main()
{
    socket_creation();
    return 0;
}

//Dúvidas:
/*
    Tratar o socket na thread
    Criar uma threaad ou varias igual o npthreads?
*/

void socket_creation()
{
    //criar socket -> bind -> listen -> connection
    int socket_name = 0, new_conn = 0, option = 1;
    struct sockaddr_in serv_addr;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int i = 0;
    char sendBuff[1024];

    socket_name = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_name < 0)
    {
        printf("%s","Failed to create socket, exiting\n");
    }else{

        printf("%s", "Created socket!\n");
        
    }

    if (setsockopt(socket_name, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option))) {
        printf("Socket error");
        exit(1);
    }
    // Cleaning serv_addr
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    if (bind(socket_name, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding\n");
      exit(1);
   }

   /*  if (listen(socket_name, 10)){
        perror("ERROR on listening\n");
        exit(1);
    } */

    //Running multiple threads
    for (i = 0; i < N_THREAD; i++) {
        //Listen capacity
        if (listen(socket_name, 10)){
            perror("ERROR on listening\n");
            exit(1);
        }
        pthread_create(&threads[i], NULL, thread_tratadora, (void *) accept(socket_name, (struct sockaddr *)&address, (socklen_t *)&addrlen));
	
    }
    //close(new_conn);
        
}