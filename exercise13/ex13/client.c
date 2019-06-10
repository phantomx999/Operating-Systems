#define _BSD_SOURCE
#define NUM_ARGS 3

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERVER_PORT 4061


int main(int argc, char** argv) {

	if (argc < NUM_ARGS + 1) {

		printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
		exit(1);
	}

	// Create a TCP socket.
	int sock = socket(AF_INET , SOCK_STREAM , 0);
	
	// Specify an address to connect to (we use the local host or 'loop-back' address).
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(SERVER_PORT);
	address.sin_addr.s_addr = inet_addr(argv[1]);

	//initialize the messages to send
	//struct sendInfo* acct =  malloc(sizeof(struct sendInfo));
	//acct->acctno = atoi(argv[2]);
	//acct->blceChange = atof(argv[3]);
	
	char * blceBuf = malloc(sizeof(char)*1024);
	
	
	// Connect it.
	if (connect(sock, (struct sockaddr *) &address, sizeof(address)) == 0) {
		int bytes_sent;
		// TODO: Write data to socket.
		bytes_sent = send(sock, argv[2], sizeof(argv[2]), 0);
		bytes_sent = send(sock, argv[3], sizeof(argv[3]), 0);

		// TODO: Read the data sent back.
		int bytes_recv = recv(sock, blceBuf, 1024, 0);
		printf("Account: %s, Balance: %s\n",argv[2],blceBuf);
		// Close the socket.
		close(sock);
		
	} else {
	
		perror("Connection failed!");
	}
}
