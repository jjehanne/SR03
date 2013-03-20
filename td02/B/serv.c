#include <sys/socket.h> // accept(), bind(), connect(), listen(), recv(), send(), socket()..
#include <sys/types.h> // data types
#include <sys/wait.h> //waitpid()
#include <unistd.h> // fork(), pipe(), sleep(), read(), write()...
#include <netinet/in.h> // ntohl(), ntohs(), htonl() and htons()
#include <netdb.h> // gethostbyname()
#include <stdio.h> 
#include <signal.h>
#include "iniobj.h"

void handlerSIGCHLD(){
    int status;
    waitpid(-1, &status, WUNTRACED);
    printf("Server - Child terminated with status %d\n", status);
}

void traiterClient(int sds){

    sleep(5);

    obj liste;
    int recvData;
    if((recvData=recv(sds, &liste, sizeof(obj), 0)) < 0){
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

            recvData=recv(sds,&liste,sizeof(obj),0);

        }
    }
    close(sds); //close client socket
}

int main(int argc, char *argv[]){
    int sd, saddrl, sds, saddrCliLen;
    pid_t pid;

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
        if(pid < 0){
            perror("fork() failed");
            return 1;
        }
        else if(pid > 0){
            //processus père se remet directement en accept
        }
        else {
            //pid=0 processus fils
            printf("Traitement du client par un processus fils\n");
            traiterClient(sds);
            return 0;
        }
    }

    close(sd); //close server socket

    return 0;

    //eventuellement handler ctrl C pour close sd mais le mettre en valeur globale
}
