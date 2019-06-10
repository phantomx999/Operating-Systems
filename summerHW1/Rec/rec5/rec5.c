#define _BSD_SOURCE

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

void printStats(char* path, int max) {

	// TODO: Put code here.
}

int main(int argc, char** argv) {

	if (argc < 3) {

		printf("Wrong number of args, expected 2, given %d", argc - 1);
		exit(1);
	}

	printStats(argv[1], atoi(argv[2]));
}
