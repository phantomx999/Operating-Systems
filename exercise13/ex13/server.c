#define _BSD_SOURCE
#define NUM_ARGS 2

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
#define MAX_CONNECTIONS 100

struct bankAccount {

	double balance;
	pthread_mutex_t* mutex;
};

struct threadArg {

	int accountNumber;
	double change;
	int clientfd;
	struct bankAccount* accts;
	
};

void threadFunction(void* args) {

	struct threadArg* arg = args;
	printf("Server: original balance: %f\n", (arg->accts)[arg->accountNumber].balance);
	
	// TODO: Alter the specified account;
	pthread_mutex_lock((arg->accts)[arg->accountNumber].mutex);
	(arg->accts)[arg->accountNumber].balance = (arg->accts)[arg->accountNumber].balance + arg -> change;
	pthread_mutex_unlock((arg->accts)[arg->accountNumber].mutex);
	// TODO: Write back to client.
	int bytes_sent = send(arg->clientfd, &((arg->accts)[arg->accountNumber].balance), sizeof((arg->accts)[arg->accountNumber].balance), 0);
	
	// Close client connection.
	close(arg->clientfd);
	printf("Server: changed balance: %f\n", (arg->accts)[arg->accountNumber].balance);
	
	free(args);
	
}

int main(int argc, char** argv) {

	if (argc > NUM_ARGS + 1) {

		printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
		exit(1);
	}
	
	// Create a TCP socket.
	int sock = socket(AF_INET , SOCK_STREAM , 0);
	
	// Bind it to a local address.
	struct sockaddr_in servAddress;
	servAddress.sin_family = AF_INET;
	servAddress.sin_port = htons(SERVER_PORT);
	servAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sock, (struct sockaddr *) &servAddress, sizeof(servAddress));
	
	// We must now listen on this port.
	listen(sock, MAX_CONNECTIONS);
	
	// TODO: Init bank accounts.
	
	// TODO: Init threads.
	int num_thrd = atoi(argv[2]);
	int num_acct = atoi(argv[1]);
	
	pthread_t pool[num_thrd];
	struct bankAccount* b[num_acct];
	
	// Create the mutexes
	pthread_mutex_t* locks[num_acct];
	int l;
	for (l=0; l < num_acct; ++l) {
	
		locks[l] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(locks[l], NULL);
	}
	
	// Create the account.
	int i;
	for (i=0; i < num_acct; ++i) {
	
		b[i] =  malloc(sizeof(struct bankAccount));
		b[i]->balance = 0.0;
		b[i]->mutex = locks[i];
	
	}
	static int threadCt = 0;
	// A server typically runs infinitely, with some boolean flag to terminate.
	while (1) {
		
		// Now accept the incoming connections.
		struct sockaddr_in clientAddress;
		
		socklen_t size = sizeof(struct sockaddr_in);
		int clientfd = accept(sock, (struct sockaddr*) &clientAddress, &size);
		threadCt++;
		int bytes_recv;
		// TODO: Read request from the socket.
		char * acctBuf = malloc(sizeof(char)*1024);
		char * blceBuf = malloc(sizeof(char)*1024);
		bytes_recv = recv(clientfd, acctBuf, 1024, 0);
		bytes_recv = recv(clientfd, blceBuf, 1024, 0);
		// TODO: Run a thread to handle the request.
		struct threadArg * ta =  malloc(sizeof(struct threadArg));
		ta->accountNumber = atoi(acctBuf);
		ta->change = atof(blceBuf);
		ta->clientfd = clientfd;
		ta->accts = b;

		pthread_create(&pool[threadCt], NULL, threadFunction, (void*) ta);
		pthread_detach(&pool[threadCt]);

		

	}
	
	// Close the socket.
	close(sock);
}
