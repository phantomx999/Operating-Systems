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
#define MAX_LINE 1024

int line_count = 1;
char** candidate_names;
int number_nodes = 0;
int number_candidates = 0;
char** node_names;

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
	FILE* f = file_open(filename);
	char* buff = malloc((sizeof(char) * (MAX_LINE + 1)));
	if(f == NULL){
		return 0;
	}
	buff = read_line(buff, f);
	trimwhitespace(buff);
	while ( (buff[0] == '#') || (buff[0] == "")){
		buff = read_line(buff, f);	
	}
	while( (buff != NULL) && (buff > 0)){
		if(line_count == 1){		
			number_candidates = atoi(buff[0]);				
			int temp = parseInputLine(buff, n);
		}
		else{
			int temp = parseInputLine(buff, n);
			buff = read_line(buff, f);
			trimwhitespace(buff);
			while (buff[0] == '#' || buff[0] == ""){
				buff = read_line(buff, f);	
			}
		}
	}
	char* temp = "";
	char* temp2 = "";
	for(int j = number_candidates - 1; j >= 0; j--){
		strcpy(temp, candidate_names[j]);
		prepend(temp, " ");
		prepend(temp2, temp);
		temp = "";
	} 

	for(int i = 0; i < number_nodes; i++){
		if(n[i].status == -2){
			strcpy(n[i].input, n[i].name);
			strcpy(n[i].output, n[i].name);
			n[i].num_children = 0;
			n[i].status = -1;
			prepend(n[i].output, "Output_");
			strcpy(n[i].prog, temp2);
			char c = number_candidates + '0';
			prepend(n[i].prog, c);
			prepend(n[i].prog, " ");
			prepend(n[i].prog, n[i].output);
			prepend(n[i].prog, " ");
			for(int k = n[i].num_children - 1; k >= 0; k--){
				prepend(n[i].prog, n[k].input);
				prepend(n[i].prog, " "); 
			}
			prepend(n[i].prog, "./leafcounter");
		}
		else if(strcmp(n[i].name, "Who_Won") == 0){
			strcpy(n[i].prog, temp2); 
			char c = number_candidates + '0';
			prepend(n[i].prog, c);
			prepend(n[i].prog, " ");
			prepend(n[i].prog, n[i].output);
			prepend(n[i].prog, " ");
			for(int k = n[i].num_children - 1; k >= 0; k--){
				prepend(n[i].prog, n[k].input);
				prepend(n[i].prog, " "); 
			}
			prepend(n[i].prog, n[i].num_children);
			if(strcmp(n[i].name, "Who_Won") == 0){
				prepend(n[i].prog, "./find_winner ");
			}
			else{
				prepend(n[i].prog, "./aggregate_votes ");
			}
		}	
	}

	return number_nodes;	
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
		
}

int parseInputLine(char *s, node_t *n);
int parseInputLine(char *s, node_t *n){
	if(line_count == 1){
		candidate_names = malloc(sizeof(char*) * number_candidates);
		char** strings = malloc(sizeof(char*) * number_candidates + 1);
		if( (makeargv(trimwhitespace(s), ' ', &strings)) == 0){
			return 0;
		}
		for(int i = 0; i<number_candidates; i++){
			strcpy(candidate_names[i], trimwhitespace(strings[i+1]));
		}
		free(strings);		
		line_count++;
		return 0;
	}
	else if(line_count == 2){
		char** strings = malloc(sizeof(char*) * MAX_NODES);
		number_nodes = makeargv(trimwhitespace(s), ' ', &strings);
		if(number_nodes == 0){
			return 0;
		}
		for(int i = 0; i < number_nodes; i++){
			strcpy(n[i].name, trimwhitespace(strings[i]));
			n[i].id = i+1;
			n[i].status = -2;
		}
		free(strings);
		return 0;		
	}
	char** strings = malloc(sizeof(char*) * number_nodes);
	char** childr = malloc(sizeof(char*) * number_nodes);
	char* parent_name;
	makeargv(trimwhitespace(s), ':', &strings);
	parent_name = strings[0];
	int num_child = 0;
	num_child = makeargv(trimwhitespace(strings[1]), ' ', &childr);
	node_t* parent = findnode(n, parent_name); 
	parent->num_children = num_child;
	char* prepend_name = prepend(parent_name, "Output_");
	strcpy(parent->output, prepend_name);
	parent->status = 1;
	for(int i = 0; i < num_child; i++){
		node_t* child = findnode(n, trimwhitespace(childr[i]));
		parent->children[i] = child->id;
		char* prepend_name = prepend(child->name, "Output_");
		strcpy(parent->input[i], prepend_name);
	}
	free(strings);
	free(childr);
	return num_child;
	
		
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
	if(n->num_children == 0){
		char **progArray = malloc(sizeof(char*)*MAX_NODES);
		int numArg = makeargv(trimwhitespace(n->prog), " ", &progArray);
		progArray[numArg] = NULL;
		execv("leafcounter", progArray);
		fprintf(stderr, "%s failed to exec leafcount\n", n->name);	
	}
	else if(n->num_children > 0){
		pid_t pid;
		for(int i = 0; i < n->num_children; i++){
			pid = fork();
			if(pid == -1){
				fprintf(stderr, "Fail to fork with parent: %s\n", n->name);
				exit(1);
			}
			else if(pid == 0){
				node_t* temp = findNodeByID(n, n->children[i]);
				temp->pid = getpid();
				execNodes(temp);
			}
			else{
				pid_t waitpid;
				while(((waitpid = wait(NULL)) == -1) && (errno == EINTR)){
					if(pid != waitpid){
						fprintf(stderr, "Failed to wait due to error");
						exit(1);
					}
				}
			}		
		}
	}
	char** progArray2 = malloc(sizeof(char*)*MAX_NODES);
	int numArgs2 = makeargv(trimwhitespace(n->name), " ", &progArray2);
	progArray2[numArgs2] = NULL; 
	if(n->id == 1){
		execv("find_winner", progArray2);
		fprintf(stderr, "%s failed to exec find_winner\n,", n->name);
		exit(1);
	}
	else{
		execv("aggregate_vote", progArray2);
		fprintf(stderr, "%s failed to exec aggregate_vote\n,", n->name);
		exit(1);	
	} 
}


int main(int argc, char **argv){

	//Allocate space for MAX_NODES to node pointer
	struct node* mainnodes=(struct node*)malloc(sizeof(struct node)*MAX_NODES);
fprintf(stderr, "made it here 1");
	if (argc != 2){
		printf("Usage: %s Program\n", argv[0]);
		return -1;
	}

	//call parseInput
	int num = parseInput(argv[1], mainnodes);
	printgraph(mainnodes, number_nodes);

	//Call execNodes on the root node
	execNodes(mainnodes);

	return 0;
}
