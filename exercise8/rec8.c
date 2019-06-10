#define _BSD_SOURCE
#define NUM_ARGS 1

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

struct threadArgs {

	int** A;
	int** B;
	int** C;
	int n;
	int row;

};

// Create a matrix of size n. DO NOT NEED TO CHANGE
int** createMatrix(int n) {

	int** result = (int**) malloc(sizeof(int*) * n);
	for (int i=0; i < n; ++i) result[i] = (int*) malloc(sizeof(int) * n);

	return result;
}

// Fill the matrix with random values. DO NOT NEED TO CHANGE
void fillRandom(int** matrix, int n) {

	for (int i=0; i < n; ++i) {
		for (int j=0; j < n; ++j) {
			matrix[i][j] = rand();
		}
	}
}

// Add the values contianed in a single row of A and B into the same row in C.
void * matrixRowAdd(void* arg) {

	// TODO: Your code here.
	struct threadArgs *ar;
	ar = (struct threadArgs *) arg;
	for(int i = 0; i < ar->n; i++){
		ar->C[ar->row][i] = ar->A[ar->row][i] + ar->B[ar->row][i];	
	}
}

int main(int argc, char** argv) {

	if (argc < NUM_ARGS + 1) {

		printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
		exit(1);
	}
	srand(time(NULL));
	int n = atoi(argv[1]);

	// Create argument matrices.
	int** A = createMatrix(n);
	int** B = createMatrix(n);
	int** C = createMatrix(n);

	// Fill args randomly.
	fillRandom(A, n);
	fillRandom(B, n);



	struct threadArgs args[n];
	pthread_t threadS[n];

	//initialize stuff
	for(int i = 0; i < n; i++){
		args[i].n = n;
		args[i].row = i;
		args[i].A = A;
		args[i].B = B;
		args[i].C = C;		
	}

	// Perform matrix addition in one thread.
	// TODO: Your code here.
	long t1 = time(NULL);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			C[i][j] = A[i][j] + B[i][j];	
		}
	        //fprintf(stderr, "in single\n");
	}

	long t2 = time(NULL);
	printf("Time (in sec) to perform single thread add = %d\n", (t2 - t1));


	// Perform matrix addition with multiple (n) threads.
	t1 = time(NULL);
	// TODO: Your code here.
	for(int i = 0; i < n; i++){
		if(pthread_create(&threadS[i], NULL, matrixRowAdd, (void*) &args[i]) != 0){
			perror("Error, pthread_create, multithread");
			return EXIT_FAILURE;
		}
		//fprintf(stderr, "make it in loop\n");
	}        
	for(int i = 0; i < n; i++){
		pthread_join(threadS[i], NULL);
	}     

        t2 = time(NULL);
        printf("Time (in sec) to perform multithreaded add = %d\n", (t2 - t1));
}











