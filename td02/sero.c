#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include "iniboj.h"

int main(int argc, char *argv[]){
    int sd, saddrl, sds, saddrcl;
    pid_t pid;

    struc sockaddr_in saddrser, saddrcli;

    sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    saddrl = sizeof(saddrser);
    bzero(&saddrser, saddrl);
    saddrser.sin_family = AF_INET;
    saddrser.sin_port = htons(atoi(argv[1]));
    saddrser.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sd, (const struct sockaddr*) &saddrser, saddrl);
    listen(sd,5);
    saddrcl = sizeof(saddrcli)
    sds = accept(sd, saddrcli, saddrcl);

    pid = fork();
    if(pid < 0)
        perror("fail fork");
    else if(pid > 0){
        traiterclient();
    }
    else {

    }
    return 0;

}
