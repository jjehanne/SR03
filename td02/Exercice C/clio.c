#include <stdio.h> // printf()...
#include <stdlib.h> // malloc()
#include <string.h> // strlen(), strcpy()...
#include <strings.h> // bcopy(), bzero()
#include <unistd.h> // fork(), pipe(), sleep(), read(), write()...
#include <sys/socket.h> // accept(), bind(), connect(), listen(), recv(), send(), socket()..
#include <sys/types.h> // data types
#include <signal.h> // signal() handling function
#include <netinet/in.h> // ntohl(), ntohs(), htonl() and htons()
#include <netdb.h> // gethostbyname()

#include "iniobj.h" // liste d'objets


int main(int argc, char *argv[]){

    if(argc < 3)
    {
        fprintf(stderr,"Usage: %s <Server IP> <PORT>\n",argv[0]);
        exit(-1);
    }

    struct sockaddr_in saddrserv; // server adress
    struct hostent * hid; //
    int sd; //socket descriptor
    int saddrl;
    int i=0;

    saddrl = sizeof(saddrserv);

    obj* liste = iniobj();
    if (liste == NULL){
        printf("Erreur d'initialisation de la liste");
        return -1;
    }

    if ((sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("socket() failed");
        exit(-1);
    }
    
    bzero(&saddrserv, saddrl);
    hid = gethostbyname(argv[1]);
    saddrserv.sin_family = AF_INET;
    saddrserv.sin_port = htons(atoi(argv[2]));
    bcopy(hid->h_addr, &(saddrserv.sin_addr.s_addr), saddrl);
    connect(sd, (struct sockaddr *) &saddrserv, saddrl);

    for(i=0; liste[i-1].iqt != -1; i++){
        if(send(sd,&liste[i],sizeof(obj),0) != sizeof(obj)){

            perror("Client : Error sending object");
            exit(-1);
        } 
        else {
            printf("Client - Object #%d successfully sent.\n",i);
            if(liste[i].iqt == -1) printf("Token iqt detected, all objects sent.\n");
        }
    };

    return 0;

}
