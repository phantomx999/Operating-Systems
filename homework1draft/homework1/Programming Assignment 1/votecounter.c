/*
 * VCforStudents.c
 *
 *  Created on: Feb 2, 2018
 *      Author: ayushi
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "makeargv.h"

#define MAX_NODES 100
#define MAX_CANDIDATES 10

int number_of_candidates = 0;
char** candidate_names;
char* strings[MAX_NODES][MAX_NODES]; 
int number_of_nodes = 0;
int line_number = -1;
//Function signatures

/**Function : parseInput
 * Arguments: 'filename' - name of the input file
 * 			  'n' - Pointer to Nodes to be allocated by parsing
 * Output: Number of Total Allocated Nodes
 * About parseInput: parseInput is supposed to
 * 1) Open the Input File [There is a utility function provided in utility handbook]
 * 2) Read it line by line : Ignore the empty lines [There is a utility function provided in utility handbook]
 * 3) Call parseInputLine(..) on each one of these lines
 ..After all lines are parsed(and the DAG created)
 4) Assign node->"prog" ie, the commands that each of the nodes has to execute
 For Leaf Nodes: ./leafcounter <arguments> is the command to be executed.
 Please refer to the utility handbook for more details.
 For Non-Leaf Nodes, that are not the root node(ie, the node which declares the winner):
 ./aggregate_votes <arguments> is the application to be executed. [Refer utility handbook]
 For the Node which declares the winner:
 This gets run only once, after all other nodes are done executing
 It uses: ./find_winner <arguments> [Refer utility handbook]
 */
int parseInput(char *filename, node_t *n);

int parseInput(char *filename, node_t *n){
	char* file = "input.txt";
	FILE* f = file_open(file);
	if(f != NULL){
		fseek(f, 0, SEEK_END);
		size = ftell(fp);
		if(size == 0){
			return 0;
		}
	}
	char* buffer = malloc(sizeof(char * 1024);
	line_number = 1;
	int temp = 0;
	buffer = read_line(buffer, f);
	if(buffer == 0){
		return 0;
	}

	while(buffer != NULL && buffer > 0 && buffer[0] != '#'){		
		if(line_number == 1){	
			number_of_candidates = atoi(buffer[0]);
			printf("buffer: %s\n" buffer),		
			temp = parseInputLine(buffer, n);
		}
		else if(line_number == 2){
			printf("buffer: %s\n", buffer);
			temp = parseInputLine(buffer, n);
		}
		else{
			printf("buffer: %s\n", buffer);
			temp = parseInputLine(buffer, n);
		}
		line_number++;
		buffer = read_line(buffer, f);
	}
	if(temp == 0){
		return 0;
	}
	for(int i = 0; i < number_of_nodes; i++){
		if(n[i].status == -2){
			strcpy(n[i].input[0], n[i].name);
			strcpy(n[i].output, n[i].name);
			prepend(n[i].output, "Output_");
			n[i].num_children = 0;
			n[i].status = -1;
		}
		if(n[i].status == 1 && i != 0){
			int m;
			char* inputs = malloc(1024);
			for(m = 0; m<n[i].num_children; m++){
				strcat(inputs, n[i].input[m];
			}
		}
	}
	return number_of_nodes;
}

/**Function : parseInputLine
 * Arguments: 's' - Line to be parsed
 * 			  'n' - Pointer to Nodes to be allocated by parsing
 * Output: Number of Region Nodes allocated
 * About parseInputLine: parseInputLine is supposed to
 * 1) Split the Input file [Hint: Use makeargv(..)]
 * 2) Recognize the line containing information of
 * candidates(You can assume this will always be the first line containing data).
 * You may want to store the candidate's information
 * 3) Recognize the line containing "All Nodes"
 * (You can assume this will always be the second line containing data)
 * 4) All the other lines containing data, will show how to connect the nodes together
 * You can choose to do this by having a pointer to other nodes, or in a list etc-
 * */



int parseInputLine(char *s, node_t *n);

int parseInputLine(char *s, node_t *n){		
	
	if(line_number == 1){
		candidate_names = malloc(sizeof(char*) * number_of_candidates);
		char** strings = malloc(sizeof(char*) * number_of_candidates + 1);
		if(makeargv(trimwhitespace(s), " ", &strings) == 0){
			return 0;
		} 
		for(int i = 0; i<number_of_candidates; i++){		
			strcpy(candidate_names[i], trimwhitespace(strings[i+1]));		
		}
		free(strings);
		return -1;
	}
	else if(line_number == 2){
		char** strArray = malloc(sizeof(char*) * MAX_NODES);
		int strings_per_line = makeargv(trimwhitespace(s), " ", &strArray)
		if(strings_per_line == 0){
			return 0;
		}
		else{
			for(int i = 0; i < number_of_candidates; i++){
				strcpy(n[i].name, trimwhitespace(strArray[i]));
				n[i].id = i+1;
				n[i].status = -2;
			}
			free(strArray);
			return -1;
			//number_of_candidates = atoi(strings[0][0]);
			//for(int i = 0; i < number_of_candidates; i++){
			//	candidate_names[i] = strings[0][i+1];
			//}
		}
		return -1;
	}
	
	//below: for any line numbers > 2 in input.txt
	char** strAr = malloc(sizeof(char*) * MAX_NODES);
	char** childArray = malloc(sizeof(char*) * MAX_NODES);
	char* parentName;
	int numChild;
	makeargv(trimwhitespace(s), ":", &strAr);
	parentName = strAr[0];
	numChild = makeargv(trimwhitespace(strAr[1]), " ", &childArray);
	node_t* parent_Node = malloc(sizeof(node_t));
	parent_Node = findNode(n, parentName);
	parent_Node -> num_children = numChild; 
	char* prepend_name = malloc(sizeof(char)*1024);
	prepend_name = parentName;
	prepend(prepend_name, "Output_");
	strcpy(parent_Node -> output, prepend_name);
	parent_Node -> status = 1;
	for(int i = 0; i < numChild; i++){
		node_t* child_Node = malloc(sizeof(node_t));
		child_Node = findnode(n, trimwhitespace(childArray[i]));

		parent_Node -> children[i] = child_Node -> id;

		snprintf(parent_Node -> input[i],1024, "%s%s"," Output_",trimwhitespace(childArray[i]));
	}

	return numChild;	
}

/**Function : execNodes
 * Arguments: 'n' - Pointer to Nodes to be allocated by parsing
 * About execNodes: parseInputLine is supposed to
 * If the node passed has children, fork and execute them first
 * Please note that processes which are independent of each other
 * can and should be running in a parallel fashion
 * */
void execNodes(node_t *n);

void execNodes(node_t *n){
	
		
	
	

}


int main(int argc, char **argv){

	//Allocate space for MAX_NODES to node pointer
	struct node* mainnodes=(struct node*)malloc(sizeof(struct node)*MAX_NODES);

	if (argc != 2){
		printf("Usage: %s Program\n", argv[0]);
		return -1;
	}

	//call parseInput
	int num = parseInput(argv[1], mainnodes);
	if(num == 0){
		printf("Error: No input, empty file");
		return(EXIT_FAILURE):
	}


	//Call execNodes on the root node


	return 0;
}
