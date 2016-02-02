/*
* Xiaoren Yang
* 
* server.c
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
#include <time.h>
#include <crypt.h>
#include "header.h"


void usage();

int main(int argc, char *argv[]) 
{
	/* parse argument */
	int c;
	int pflag = 0, hflag = 0, tflag = 0, errflag = 0;
	extern int optind, opterr;
	extern char *optarg;

	opterr = 1;				/* set to 0 to disable error message */

	char message[BUFFSIZE];
	char respond[BUFFSIZE];
	char salt[3];
	int bytes;
	int sock, serv_host_port, clilen, newsock;
	struct sockaddr_in cli_addr, serv_addr;

	int pid; 		/* process id */

	/* used for pipe */
	FILE *pp;

	printf("processing argument: %d\n", optind);
	while((c = getopt(argc, argv, "p:ht")) != EOF) {
		switch(c) {
			case 'p':
				pflag++;
				serv_host_port = atoi(optarg);
				break;
			case 'h':
				hflag++;
				break;
			case 't':
				tflag++;
				break;
			printf("processing argument: %d\n", optind);
		}
	}
	if(pflag){
		printf("Listen activating.\n");
		printf("Trying to create socket at port %d...\n", serv_host_port);   
	}
	else {							/* not acceptable argument */
		fprintf(stderr, "Missing one of three options \n");
		usage();
		exit(1);		
	}
	if(hflag) {
		usage();
	}

	/* create socket from which to read */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("creating socket");
		exit(1);
	}
   
	/* bind our local address so client can connect to us */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(serv_host_port);
	if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("can't bind to local address");
		exit(1);
	}

	while(1){

		/* mark socket as passive, with backlog num */
		if (listen(sock, 5) == -1) {
			perror("listen");
			exit(1);
		}

		/* wait here (block) for connection */
		clilen = sizeof(cli_addr);
		printf("wait here for accepting connection from client\n");
		newsock = accept(sock, (struct sockaddr *) &cli_addr, &clilen);
		printf("Socket ready to go! Accepting connections....\n\n");
		if (newsock < 0) {
			perror("accepting connection");
			exit(1);
		}

	   	/* fork here */
		pid = fork();
		if (pid == 0) {							/* child process */
			puts("I am the child");
			close(sock);

			/* read user-name */
			readSocket(newsock, message, BUFFSIZE);
			printf("first received: '%s'\n", message);
			
			bzero((void *)&message, sizeof(message));
			/* write a random number back to client */
			srand(time(NULL));
			sprintf(salt, "%d", rand() % 90 + 10);
			writeSocket(newsock, salt, 3);

			/* read encrypted message */
			readSocket(newsock, message, BUFFSIZE);
			printf("second received: '%s'\n", message);
			/* check password by strcmp encrypt message */
			if(strcmp(message, crypt(PASSWORD, salt)) != 0){
				writeSocket(newsock, "Wrong password", BUFFSIZE);
			}
			else {
				writeSocket(newsock, "Correct password", BUFFSIZE);				
			}

			/* tear down the command executation if -t option */
			if(tflag){
				close(newsock);
				exit(0);
			}

			/* read command from client and excecute it */
			readSocket(newsock, message, BUFFSIZE);

			/* parse the incoming command first */
			char *str = strdup(message);
			char ** res  = NULL;
			char *  p    = strtok (str, " ");
			int n_spaces = 0, i;

			/* split string and append tokens to 'res' */
			while (p) {
				res = realloc (res, sizeof (char*) * ++n_spaces);

				if (res == NULL)
				exit (-1); /* memory allocation failed */

				res[n_spaces-1] = p;

				p = strtok (NULL, " ");
			}

			/* realloc one extra element for the last NULL */

			res = realloc (res, sizeof (char*) * (n_spaces+1));
			res[n_spaces] = 0;

			/* free the memory allocated */
			dup2(newsock, STDOUT_FILENO);
			dup2(newsock, STDERR_FILENO);

			/* close connected socket and original socket */
			close(newsock);

			int eRtn = execvp(*res, res);
			if(eRtn){
				perror("execvp()");
				exit(eRtn);
			}

			free (res);

			exit(0);
		}
		else {									/* parent process */
			puts("I am the parent");
			close(newsock);
		}

	}

	close(sock);

}

void usage() {
	fprintf(stderr, "\nUsage: ./server -p <port> [-h]\n");
	fprintf(stderr, "\t-p <port>\t- port\n");
	fprintf(stderr, "\t[-h] optional argument, ask for display usage\n\n");
}

