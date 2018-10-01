#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#define N_THREAD 5
#define MSG_LEN 1024

int con = 0;
pthread_t threads[N_THREAD];
pthread_mutex_t mutex;
char msg_recv[MSG_LEN];
char server_resposta[MSG_LEN];

void *thread_tratadora(void * args)
{
    int k, msg, id;
    int conn = (int *) args;
    char tempout[MSG_LEN];
    char * pch;
    printf("\nConection made\n");
    memset(&msg_recv, '\0', sizeof(msg_recv));
    //Thread roda eterno até sair
    while(1)
    {
        msg = read(conn, msg_recv, sizeof(msg_recv));
        /* id = identifica(msg_recv); */
        printf("\n[%i]cmd: ", conn);
        printf("%s", msg_recv);
        if(!strncmp(msg_recv, "ls", 2))//comando ls
        {
            printf("LISTAGEM DOS ARQUIVOS\n");
            system("ls");
            strncpy(server_resposta, " - - - - - > ls executado no server < - - - - - \n", 0);
            write(conn, server_resposta, sizeof(server_resposta));
            send(conn, server_resposta, sizeof(server_resposta), 0);
            //execl("/bin/ls",  "ls", NULL);
        }else if(!strncmp(msg_recv, "touch", 5))//comando touch
        {   
            pthread_mutex_lock(&mutex);
            printf("CRIANDO ARQUIVO\n");
            /* char *lastToken;
            pch = strtok(msg_recv, " ");
            while (pch != NULL)
            {   
                lastToken = pch;
                pch = strtok (NULL, " ");
            }
            printf("%s\n", lastToken);
            execl("/bin/touch", "touch", lastToken, NULL); */
            system(msg_recv);
            strncpy(server_resposta, " - - - - - > touch executado no server < - - - - - \n", 0);
            send(conn, server_resposta, sizeof(server_resposta), 0);
            pthread_mutex_unlock(&mutex);

        }else if(!strncmp(msg_recv, "gedit", 5))//comando abrir editor, n precisava
        {
            printf("EDITANDO ARQUIVO\n");
            pthread_mutex_lock(&mutex);
            system(msg_recv);
            strncpy(server_resposta, " - - - - - > gedit aberto para edição < - - - - - \n", 0);
            send(conn, server_resposta, sizeof(server_resposta), 0);
            pthread_mutex_unlock(&mutex);
        }else if(!strncmp(msg_recv, "rm -r",5))//comando remover arquivo/diretorio
        {
            printf("REMOVENDO ARQUIVO\n");
            pthread_mutex_lock(&mutex);
            system(msg_recv);
            strncpy(server_resposta, " - - - - - > arquivo removido server side < - - - - - \n", 0);
            send(conn, server_resposta, sizeof(server_resposta), 0);
            pthread_mutex_unlock(&mutex);
        }else if(!strncmp(msg_recv, "mkdir",5))//comando mkdir
        {
            printf("DIRETÓRIO CRIADO\n");
            pthread_mutex_lock(&mutex);
            system(msg_recv);
            strncpy(server_resposta, " - - - - - > dir criado < - - - - - \n", 0);
            send(conn, server_resposta, sizeof(server_resposta), 0);
            pthread_mutex_unlock(&mutex);
        }
        else if(!strncmp(msg_recv, "cd",2))//comando alterar diretorio N FUNFA
        {
            //erro ainda!!
            printf("DIRETÓRIO ALTERADO\n");
            pthread_mutex_lock(&mutex);
            system(msg_recv);
            system("pwd");
            strncpy(server_resposta, " - - - - - > dir alterado < - - - - - \n", 0);
            send(conn, server_resposta, sizeof(server_resposta), 0);
            pthread_mutex_unlock(&mutex);
        }
        else if(!strncmp(msg_recv, "cat",3))//comando cat, mostrar no terminal conteudo
        {
            printf("CONTEÚDO DENTRO DO ARQUIVO\n\n");
            pthread_mutex_lock(&mutex);
            system(msg_recv);
            strncpy(server_resposta, " - - - - - > leitura no terminal server < - - - - - \n", 0);
            send(conn, server_resposta, sizeof(server_resposta), 0);
            pthread_mutex_unlock(&mutex);
        }else if(!strncmp(msg_recv, "echo",4))//comando echo, escreve dentro do arquivo
        {
            printf("ADICIONANDO AO ARQUIVO\n");
            pthread_mutex_lock(&mutex);
            system(msg_recv);
            strncpy(server_resposta, " - - - - - > escrito no arq < - - - - - \n", 0);
            send(conn, server_resposta, sizeof(server_resposta), 0);
            pthread_mutex_unlock(&mutex);
        }

        //Zera string
        memset(&msg_recv, '\0', sizeof(msg_recv));
    }
	pthread_exit(0);
}

void socket_creation();
/* int identifica(char mensagem[1024]); */

int main()
{
    pthread_mutex_init(&mutex, NULL);
    socket_creation();
    pthread_mutex_destroy(&mutex);
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
    //Config inicial
    int socket_name = 0, new_conn = 0, option = 1;
    struct sockaddr_in serv_addr;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int i = 0;
    char sendBuff[1024];

    //Socket e tratamento de errros
    socket_name = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_name < 0)
    {
        printf("%s","Failed to create socket, exiting\n");
    }else{

        printf("%s", "Created socket!\n");
        
    }
    //Opcoes de porta
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

    //Running multiple threads
    for (i = 0; i < N_THREAD; i++) {
        //Listen capacity
        if (listen(socket_name, 10)){
            perror("ERROR on listening\n");
            exit(1);
        }
        pthread_create(&threads[i], NULL, thread_tratadora, (void *) accept(socket_name, (struct sockaddr *)&address, (socklen_t *)&addrlen));
	
    }
        
}

/* int identifica(char mensagem[1024]){
    int id;
    if (mensagem[0] == 'l')
    {
        id = 1;
    }else if (mensagem[0] == 't')
    {
        id = 2;
    }else if (mensagem[0] == 'c'){
        id = 3;
    }else{
        id = 0;
    }
    return id;
} */