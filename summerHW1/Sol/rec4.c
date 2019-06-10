#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {

	if (argc < 2) {

		printf("Wrong number of args, expected 1, given %d", argc - 1);
		exit(1);
	}

	int p[2];
	pipe(p);

	if (p[0] < 0) { 

		printf("Failed to create pipe!");
		exit(1);
	}

	// Spawn a child process.
	pid_t pid = fork();
	if (pid == 0) {

		char name[] = "output.txt";

		int out = open(name, O_CREAT | O_RDWR);

		// Write the filename to the pipe.
		write(p[1], name, 11);

		// Reroute output to file.
		dup2(out, 1);

		execl("./rec3.o", "rec3.o", argv[1], (char*) NULL);
	
	} else {

		if (pid < 0) {

			printf("Fork failed!");
			exit(1);
		}

		wait(NULL);

		char buff[256];

		// Read the filename from the pipe.
		int r = read(p[0], buff, 11);

		if (r < 0) {

			printf("Read failed!");
			exit(1);
		}

		// Open the file.
		chmod(buff, 0700);
		int in = open(buff, O_RDONLY);

		if (in < 0) {

			printf("Opening file failed!");
			exit(1);
		}

		// Check to make sure it is correct.
		int last = 0;
		int current;
		int boolean = 1;
		while (read(in, buff, 11) > 0) {

			current = atoi(buff);
			boolean = current > last;
		}

		// Print result!
		printf("%s\n", boolean ? "Correct!" : "Incorrect!");

		close(in);

	}
}
