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

	//call parseInput
	int num = parseInput(argv[1], mainnodes);	//read in input, create tree and node data
	printgraph(mainnodes,numNodes);			//print tree (DAG)

	//Call execNodes on the root node
	execNodes(mainnodes);				//execute processes, determine who won

	return 0;
}

