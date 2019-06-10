#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {

	if (argc < 2) {

		printf("Incorrect number of args: given %d, expected 1.\n", argc - 1);
		exit(1);
	}

	pid_t pid = fork();
	
	// First child process.
	if (pid == 0) {

		execl("./rtime.o", "./rtime.o", argv[1], (char*) NULL);
		printf("Failed to exec!");
	} else {

		// In the patent process.

		// Check for error.
		if (pid < 0) exit(1);

		// Wait for the child.
		waitpid(pid, NULL, 0);

		// Fork again.
		pid = fork();

		// Second child process.
		if (pid == 0) {

			char* const args[] = {"randtime", argv[1], NULL};
			execv("./rtime.o", args);
		} else {
			
			// Parent process.

			// Check for error.
			if (pid < 0) exit(1);

			// Wait for child.
			waitpid(pid, NULL, 0);

			// Unnecessary exit.
			exit(0);
		}
	}
}
