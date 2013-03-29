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
#include <signal.h>
#include "iniobj.h"

void traiterclient(int sock){
	sleep(1);
	obj o;
	do {

		if (recv(sock, &o, sizeof(obj),  MSG_WAITALL) < 0) {
			return;
		}

		printf("recu :\n objet %s : %s, %d, %d, %g\n\n", o.id, o.desc, o.ii, o.jj, o.dd);

	} while (o.iqt != -1);

	close(sock);
}


void handlerSigchld(){
	int statut;
	waitpid(-1, &statut, 0);
	printf("child terminated with status : %d\n", statut);

}

int main(int argc, char *argv[]){
	int sd, saddrl, sds, saddrcl;
	pid_t pid;

	struct sockaddr_in saddrser, saddrcli;
	signal(SIGCHLD, handlerSigchld);
	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	saddrl = sizeof(struct sockaddr_in);
	bzero(&saddrser, saddrl);
	saddrser.sin_family = AF_INET;
	saddrser.sin_port = htons(atoi(argv[1]));
	saddrser.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sd, (const struct sockaddr*) &saddrser, saddrl);
	listen(sd,5);
	saddrcl = sizeof(saddrcli);

	while (1) {
		sds = accept(sd, (struct sockaddr*)&saddrcli, &saddrcl);
		pid = fork();
		if (pid < 0) {
			perror("fail fork");
			return 1;
		} else if (pid == 0) {
			traiterclient(sds);
			return 0;
		}


	}
	return 0;
 
}

