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

	int count = 0;

	char* names[max];
	struct stat* stats[max];

	DIR* dir = opendir(path);
	struct dirent* entry;

	while ((entry = readdir(dir)) != NULL && count < max) {

		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;

		if (entry->d_type == DT_DIR) {

			char next[strlen(path) + strlen(entry->d_name) + 2];
			next[0] = '\0';
			strcat(next, path);
			strcat(next, "/");
			strcat(next, entry->d_name);
			printStats(next, max);
		} else {

			names[count] = entry->d_name;
			struct stat* s = malloc(sizeof(struct stat));
			stat(entry->d_name, s);
			stats[count] = s;
			++count;
		}
	}

	long long totalSize = 0;
	for (int i=0; i < count; ++i) {

		totalSize += stats[i]->st_size;
	}

	for (int i=0; i < count; ++i) {

		printf("File: %s\n\tOwner: %d\n\tSize: %f\n\tTime since access (sec): %d\n",
			names[i], stats[i]->st_uid, (double) stats[i]->st_size / (double) totalSize, time(NULL) - stats[i]->st_atime);
	}
}

int main(int argc, char** argv) {

	if (argc < 3) {

		printf("Wrong number of args, expected 2, given %d", argc - 1);
		exit(1);
	}

	printStats(argv[1], atoi(argv[2]));
}
