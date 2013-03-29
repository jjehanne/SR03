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

void reception_serveur(int sock){
	obj object;
	do {
		// receveption of string's length
		size_t taille;
		ssize_t ret = recv(sock, &taille, sizeof(size_t),  MSG_WAITALL);
		if (ret == -1) {
			perror("erreur recv\n");
			return;
		}
		// receveption of string
		char* buffer = (char*)malloc(taille+1);
		ret = recv(sock, buffer, taille,  MSG_WAITALL);
		if (ret == -1) {
			perror("erreur recv\n");
			return;
		}
		buffer[taille] = '\0';
		// constuction of object
		size_t idSize, descSize;
		int test = sscanf(buffer, "[%03d]", &idSize);
		if (test != 1){
			perror("erreur sscanf\n");
			return;
		}
		memcpy(object.id, buffer+6, idSize);
		object.id[idSize] = '\0';
		test = sscanf(buffer+7+idSize, "[%03d]", &descSize);
		if (test != 1){
			perror("erreur sscanf\n");
			return;
		}
		memcpy(object.desc, buffer+13+idSize, descSize);
		object.desc[descSize] = '\0';
		test = sscanf(buffer+14+idSize+descSize, "%d,%d,%lg,%d", &object.ii, &object.jj, &(object.dd), &object.iqt);
		if (test != 4){
			perror("erreur sscanf\n");
			return;
		}
		free(buffer);
		printf("recu :\n objet %s : %s, %d, %d, %g\n\n", object.id, object.desc, object.ii, object.jj, object.dd);
	} while(object.iqt != -1);
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

	reception_serveur(sd);

	close(sd);
	return 0;
}
