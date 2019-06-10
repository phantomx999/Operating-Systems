#define _BSD_SOURCE
#define NUM_ARGS 3

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

char* createFilename(char* path, char* name) {

	char* fname = (char*) malloc(strlen(path) + strlen(name) + 2);
	fname[0] = '\0';
	strcat(fname, path);
	strcat(fname, "/");
	strcat(fname, name);

	return fname;
}

int myCP(char* oldFile, char* newFile) {

	int oldFD = open(oldFile, O_RDONLY);
	int newFD = open(newFile, O_CREAT | O_RDWR);

	char buff[512];
	int status = 0;
	while ((status = read(oldFD, buff, 512)) > 0) write(newFD, buff, 512);

	return status;
}

void shallowCopy(char* old, char* new) {

	// Create links to all files in old in new.
	struct dirent* entry;
	DIR* dir = opendir(old);

	while ((entry = readdir(dir)) != NULL) {

		if (entry->d_type == DT_DIR) continue;

		char * oldName = createFilename(old, entry->d_name);
		char * newName = createFilename(new, entry->d_name);

		link(oldName, newName);

		chmod(newName, 0700);

		// Reclaim memory.
		free(oldName);
		free(newName);
	}
}

void deepCopy(char* old, char* new) {

	// Copy the contents of all files in old to new.
	struct dirent* entry;
	DIR* dir = opendir(old);

	while ((entry = readdir(dir)) != NULL) {

		if (entry->d_type == DT_DIR) continue;

		char * oldName = createFilename(old, entry->d_name);
		char * newName = createFilename(new, entry->d_name);
	
		myCP(oldName, newName);

		chmod(newName, 0700);

		// Reclaim memory.
		free(oldName);
		free(newName);
	}
}

int main(int argc, char** argv) {

	if (argc < NUM_ARGS + 1) {

		printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
		exit(1);
	}

	// Make the directory, in case it doesn't exist.
	mkdir(argv[2], 0700);
	mkdir(argv[3], 0700);

	long t1 = time(NULL);
	shallowCopy(argv[1], argv[2]);
	long t2 = time(NULL);

	long t3 = time(NULL);
	deepCopy(argv[1], argv[3]);
	long t4 = time(NULL);

	printf("Time for shallow copy = %ld\n", t2 - t1);
	printf("Time for deep copy = %ld\n", t4 - t3);
}
