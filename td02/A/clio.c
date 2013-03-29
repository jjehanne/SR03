#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "iniobj.h"

void envoi_serveur(int sock){
	int i;
	for (i = 0; i < TABLEN; i++){
		send(sock, &(tab[i]), sizeof(obj), 0);
	}
}

int main(int argc, char *argv[]){
	struct sockaddr_in saddrcli, saddrserv;
	struct hostent * hid;
	int sd, saddrl;

	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	saddrl = sizeof(struct sockaddr_in);
	bzero(&saddrcli, saddrl);

	hid = gethostbyname(argv[1]);
	saddrserv.sin_family = AF_INET;
	saddrserv.sin_port = htons(atoi(argv[2]));    
	bcopy(hid->h_addr, &(saddrserv.sin_addr.s_addr), saddrl);
	connect(sd, (struct sockaddr *) &saddrserv, saddrl);

	envoi_serveur(sd);
	
	close(sd);
	return 0;
}
