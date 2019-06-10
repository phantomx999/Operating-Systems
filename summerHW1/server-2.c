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
};

struct bankAccount* accounts;
int numAccounts;

double transaction(struct bankAccount* account, double change) {

	pthread_mutex_lock(account->mutex);
	account->balance += change;
	double current = account->balance;
	pthread_mutex_unlock(account->mutex);

	return current;
}

void threadFunction(void* args) {

	struct threadArg* arg = args;

	// Alter the specified account;
	double ret = arg->accountNumber < 100 ? transaction(&accounts[arg->accountNumber], arg->change) : -1;
	
	// Write back to client.
	int bytes = write(arg->clientfd, &ret, sizeof(ret));

	if (bytes < sizeof(ret)) perror("Failed to send back to client!");
	
	close(arg->clientfd);
	
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
	
	// Init bank accounts.
	numAccounts = atoi(argv[1]);
	accounts = (struct bankAccount*) malloc(sizeof(struct bankAccount) * numAccounts);
	for (int i=0; i < numAccounts; ++i) {
	
		accounts[i].mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(accounts[i].mutex, NULL);
	}
	
	// Init threads.
	int size = atoi(argv[2]);
	pthread_t threads[size];
	int count = 0;

	// A server typically runs infinitely, with some boolean flag to terminate.
	while (1) {
		
		// Now accept the incoming connections.
		struct sockaddr_in clientAddress;
		
		socklen_t size = sizeof(struct sockaddr_in);
		int clientfd = accept(sock, (struct sockaddr*) &clientAddress, &size);
		
		// Buffer for data.
		double buffer[2];
		
		// Read from the socket and print the contents/
		int bytes = read(clientfd, &buffer, sizeof(buffer));

		if (bytes == sizeof(buffer)) {

			struct threadArg* arg = (struct threadArg*) malloc(sizeof(struct threadArg));
			arg->accountNumber = (int) buffer[0];
			arg->change = buffer[1];
			arg->clientfd = clientfd;
			pthread_create(&threads[count], NULL, threadFunction, (void*) arg);
			count = (count + 1) % size;

		} else {

			perror("Failed to parse out data!");
		}
	}
	
	// Close the socket.
	close(sock);
}
