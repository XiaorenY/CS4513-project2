/*
* Xiaoren Yang
* 
* client.c
* 
* Reference code from Mark Claypool 
* 
*/
#include <stdio.h>
#include <string.h>
#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <crypt.h>
#include "header.h"

void usage();

int main(int argc, char *argv[])
{
	/* parse argument */
	int c; 						
	int cflag = 0, sflag = 0, hflag = 0, pflag = 0, errflag = 0;
	extern int optind, opterr;
	extern char *optarg;

	opterr = 1; 		/* set to 0 to disable error message */

	char buf[80];
	char message[1024];
	int bytes;
	unsigned long int inaddr;
	int sock, serv_host_port;
	struct sockaddr_in serv_addr;
	char *serv_host_addr;
	struct hostent *hp;

	char *command;

	printf("processing argument: %d\n", optind);
	while((c = getopt(argc, argv, "c:s:p:h")) != EOF) {
		switch(c) {
			case 'c':
				cflag++;
				command = optarg;
				break;
			case 's':
				sflag++;
				serv_host_addr = optarg;
				break;
			case 'p':
				pflag++;
				serv_host_port = atoi(optarg);
				break;
			case 'h':
				hflag++;
				break;				
			printf("processing argument: %d\n", optind);
		}
	}
	if(cflag && sflag && pflag) {			/* argument is acceptable */
		printf("Talk activated.\n\n");
		printf("Trying to connect to server %s at port %d...\n", serv_host_addr, serv_host_port);
	}
	else {									/* missing one of three options */
		fprintf(stderr, "Missing one of three options \n");
		usage();
		exit(1);
	}
	if(hflag) {
		usage();
	}

	// serv_host_addr = argv[1];
 //   	serv_host_port = atoi(argv[2]);


   	/* lookup hostname.
    * (Note - gethostbyname() deprecated, could use getaddrinfo())
    */
	bzero((void *) &serv_addr, sizeof(serv_addr));
	printf("Looking up %s...\n", serv_host_addr);
	if ((hp = gethostbyname(serv_host_addr)) == NULL) {
		perror("host name error");
		exit(1);
	}
	bcopy(hp->h_addr, (char *) &serv_addr.sin_addr, hp->h_length);

	printf("Found it.  Setting port connection to %d...\n", serv_host_port);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(serv_host_port);
	/* serv_addr.sin_addr.s_addr = inet_addr(serv_host_addr); */

	/* create a TCP socket (an Internet stream socket). */
	puts("Done. Creating socket...");
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("creating socket");
		exit(1);
	}

	/* socket created, so connect to the server */
	puts("Created. Trying connection to server...");
	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("can't connect");
		exit(1);
	}

	printf("Connection established!\n");
	printf("Type in messages to send to server.\n");

	/* sends in user-name */
	writeSocket(sock, USERNAME, BUFFSIZE);

	/* read data until no more */
	readSocket(sock, message, 3);
	printf("first received: '%s'\n", message);

	/* send encrypted value back */
	writeSocket(sock, crypt(PASSWORD, message), BUFFSIZE);

	readSocket(sock, message, BUFFSIZE);
	printf("received: %s\n", message);

	/* read from stdin, sending to server, until quit */
	while (fgets(buf, 80, stdin)) {
		buf[strlen(buf)-1] = '\0'; /* remove last \n */
		printf("sending: '%s'\n", buf);
		if (write(sock, buf, strlen(buf)) == -1) {
			perror("write failed");
			break;
		}
	}

	/* close socket */
	close(sock);

	return 0;
}

void usage() {
	fprintf(stderr, "\nUsage: ./client -c <command> -s <host> -p <port> [-h]\n");
	fprintf(stderr, "\t-c <command>\t- command asked to excecute\n");
	fprintf(stderr, "\t-s <host>\t- Internet name of server host\n");
	fprintf(stderr, "\t-p <port>\t- port\n");
	fprintf(stderr, "\t[-h] optional argument, ask for display usage\n\n");
}



