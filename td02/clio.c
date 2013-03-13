#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>

int main(){
	struct sockaddr_in saddrcli, saddrserv;
	struct hostent * hid;
	int sd, saddrl;

	sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	saddrl = sizeof(saddrcli);
	bzero(&saddrcli, saddrl);

	bzero(&saddrserv, saddrl);
	hid = gethostbyname(argv[1]);
	saddrserv.sin_family = AF_NET;
	saddrserv.sin_port = htons(atoi(argv[2]));
	bcopy(hid->h_addr, &(saddrserv.sin_addr.s_addr));
	connect(sd, (struct sockaddr *) &saddrserv, saddrl);

}