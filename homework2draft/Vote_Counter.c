#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include "makeargv.h"

#define MAX_NODES 100
#define MAX_LINE 1024

int main(int argc, char **argv){	//main driver

	//Allocate space for MAX_NODES to node pointer
	//struct node* mainnodes=(struct node*)malloc(sizeof(struct node)*MAX_NODES);

	if (argc != 2){		//incorrect number of arguments
		printf("Usage: %s Program\n", argv[0]);
		return -1;
	}

	int len = 0;
	struct dirent * dir;
	DIR * p_dir;

	p_dir = opendir(argv[1]);
	if(p_dir == NULL){
		fprintf(stderr, "Cannot open directory %s\n", argv[1]);
		return(Exit_Failure);
	}

	while((dir == readdir(p_dir) != NULL){
		if(

	}

}

