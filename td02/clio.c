#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int arc, char *argv[]){
	struct sockaddr_in saddrcli, saddrserv;
	struct hostent * hid;
	int sd, saddrl;

	sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	saddrl = sizeof(saddrcli);
	bzero(&saddrcli, saddrl);

	bzero(&saddrserv, saddrl);
	hid = gethostbyname(argv[1]);
	saddrserv.sin_family = AF_INET;
	saddrserv.sin_port = htons(atoi(argv[2]));
	bcopy(hid->h_addr, &(saddrserv.sin_addr.s_addr), saddrl);
	connect(sd, (struct sockaddr *) &saddrserv, saddrl);

}