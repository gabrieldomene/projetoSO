#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#define N_THREAD 5

void *thread_tratadora(void *arg)
{
    pthread_t x;
    x = pthread_self();
    printf("Nova thread criada. TID = %ld!!\n", x);
    pthread_exit(0);
}

int main()
{
    //criar socket -> bind -> listen -< connection
    int socket_name = 0, new_conn = 0, option = 1;
    struct sockaddr_in serv_addr;
    char msg_recv[1024];
    int k = 0, i = 0;
    char sendBuff[1025];

    socket_name = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(socket_name, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if(socket_name < 0)
    {
        printf("%s","Failed to create socket, exiting\n");
    }else{

        printf("%s", "Created socket!\n");
        
    }
    
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    if (bind(socket_name, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding\n");
      exit(1);
   }

    if (listen(socket_name, 10)){
        perror("ERROR on listening\n");
        exit(1);
    }

    while (1)
    {   
        new_conn = accept(socket_name, (struct sockaddr*)NULL, NULL);
        printf("\nConection made\n");
        int msg = read(new_conn, msg_recv, sizeof(msg_recv));
        //printf("Mensagem recebida: %s\n", msg_recv);
      
        printf("Mensagem recebida: ");
        for(k = 0; k < msg; k++){
            printf("%c", msg_recv[k]);
        }
        pthread_t thread[N_THREAD];
        printf("Antes de criar as thread\n");
        for (i = 0; i < N_THREAD; i++)
        {
            pthread_create(&thread[i], NULL, thread_tratadora, NULL);
        }
        for (i = 0; i < N_THREAD; i++)
        {
            pthread_join(thread[i], NULL);
        }
        close(new_conn);
     }   
    
    return 0;
}