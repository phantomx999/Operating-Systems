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

// Create a matrix of size n.
int** createMatrix(int n) {

	int** result = (int**) malloc(sizeof(int*) * n);
	for (int i=0; i < n; ++i) result[i] = (int*) malloc(sizeof(int) * n);

	return result;
}

// Fill the matrix with random values.
void fillRandom(int** matrix, int n) {

	for (int i=0; i < n; ++i) {
		for (int j=0; j < n; ++j) {
			matrix[i][j] = rand();
		}
	}
}

// Add the values contianed in a single row of A and B into the same row in C.
void matrixRowAdd(void* arg) {

	struct threadArgs* args = (struct threadArgs*) arg;

	// Add matrix values into the result
	for (int i=0; i < args->n; ++i) 
		args->C[args->row][i] = args->A[args->row][i] + args->B[args->row][i];
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

	// Perform matrix addition in one thread.
	long t1 = time(NULL);
	struct threadArgs args[n];
	for (int i=0; i < n; ++i) {
		
		args[i] = (struct threadArgs) {A, B, C, n, i};
		matrixRowAdd((void*) &args[i]);
	}
	long t2 = time(NULL);
	printf("Time (in sec) to perform single thread add = %d\n", (t2 - t1));

	// Perform matrix addition with multiple (n) threads.
	t1 = time(NULL);
        pthread_t threads[n];
        for (int i=0; i < n; ++i) pthread_create(&threads[i], NULL, matrixRowAdd, (void*) &args[i]);
        for (int i=0; i < n; ++i) pthread_join(threads[i], NULL);
        t2 = time(NULL);
        printf("Time (in sec) to perform multithreaded add = %d\n", (t2 - t1));
}
