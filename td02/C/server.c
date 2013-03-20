#include <sys/socket.h> // accept(), bind(), connect(), listen(), recv(), send(), socket()..
#include <sys/types.h> // data types
#include <sys/wait.h> //waitpid()
#include <unistd.h> // fork(), pipe(), sleep(), read(), write()...
#include <netinet/in.h> // ntohl(), ntohs(), htonl() and htons()
#include <netdb.h> // gethostbyname()
#include <stdio.h> 
#include "iniobj.h"


void handlerSIGCHLD(){
    int status;
    waitpid(-1, &status, WUNTRACED);
    printf("Server - Child terminated with status %d\n", status);
}

obj* receiveData(int socket){

    sleep(3);

    obj liste;
    int k = 0;
    int recvData;
    obj* new_list = iniobj(); //recreate a list after receiving it

    if((recvData=recv(socket, &liste, sizeof(obj), 0)) < 0){
        perror("recv() failed");
        exit(1);
    } 
    else {
        while(recvData > 0){

            printf("Object received:\n");
            printf("    %s\n",liste.id);
            printf("    %s\n",liste.desc);
            printf("    %d\n",liste.ii);
            printf("    %d\n",liste.jj);
            printf("    %f\n\n",liste.dd);

            if(liste.iqt == -1) printf("End of list token received, all objects received.\n");

            //recreate a table from the data received

            strcpy(new_list[k].id,liste.id);
            strcpy(new_list[k].desc,liste.desc);
            new_list[k].ii = liste.ii;
            new_list[k].jj = liste.jj;
            new_list[k].dd = liste.dd;
            new_list[k].iqt = liste.iqt;

            k++;

            recvData=recv(socket,&liste,sizeof(obj),0);

        }
    }

    close(socket); //close client socket

    return new_list;
}

/* 
void sendData(int socket, obj* myList){

    data obj_size;
    obj_size.taille = sizeof(obj);
    obj_size.liste = myList;

}

*/

int main(int argc, char *argv[]){
    int sd, saddrl, sds, saddrCliLen;
    pid_t pid;
    int i;

    obj* new_list;

    struct sockaddr_in saddrser;
    struct sockaddr_in saddrcli;

    signal(SIGCHLD, handlerSIGCHLD);

    if (argc != 2){
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
        exit(1);
    }

    if ((sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("socket() failed");
        exit(1);
    }

    saddrl = sizeof(saddrser);

    bzero(&saddrser, saddrl);
    saddrser.sin_family = AF_INET;
    saddrser.sin_port = htons(atoi(argv[1]));
    saddrser.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((bind(sd, (const struct sockaddr *) &saddrser, saddrl)) == -1){
        perror("bind() failed");
        exit(1);
    }
    
    if (listen(sd,5) < 0){
        perror("listen() failed");
        exit(1);
    }
    while (1){
        saddrCliLen = sizeof(saddrcli);
        if ((sds = accept(sd, (struct sockaddr *) &saddrcli, &saddrCliLen)) < 0){
            perror("accept() failed");
            exit(1);
        }

        pid = fork();
        if(pid < 0)
            perror("fork() failed");
        else if(pid > 0){
            //processus père
            
        }
        else {
            printf("Traitement du client par un processus fils\n");
            new_list = receiveData(sds);

            // sendData(sd, new_list);

            for (i = 0; i < TABLEN; i++){
                printf("New Object #%d in list:\n", i);
                printf("    %s\n",new_list[i].id);
                printf("    %s\n",new_list[i].desc);
                printf("    %d\n",new_list[i].ii);
                printf("    %d\n",new_list[i].jj);
                printf("    %f\n\n",new_list[i].dd);
            }

        }
    }

    close(sd); //close server socket

    return 0;
}
