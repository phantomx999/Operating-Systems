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
char* candidates = "";

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
	char* buff = malloc((sizeof(char) * MAX_LINE));
	if(f == NULL){
		return 0;
	}
	buff = read_line(buff, f);
	if(buff == NULL){exit(1);}
	if(buff != NULL) {trimwhitespace(buff);}
	while ( (buff[0] == '#') || (buff[0] == "")){
		free(buff);
		char* buff = malloc((sizeof(char) * MAX_LINE));
		buff = read_line(buff, f);
		if(buff != NULL) {trimwhitespace(buff);}	
	}
	while( (buff != NULL) && (buff > 0)){
		if(line_count == 1){		
			number_candidates = buff[0] - '0'; 
			int temp = parseInputLine(buff, n);
			free(buff);
		}
		else{
			char* buff = malloc((sizeof(char) * MAX_LINE));
			if( (buff = read_line(buff, f)) == NULL){break;}
			if(buff != NULL) {trimwhitespace(buff);}
			while (buff != NULL && (buff[0] == '#' || buff[0] == "")){
				free(buff);
				char* buff = malloc((sizeof(char) * MAX_LINE));
				buff = read_line(buff, f);
				if(buff != NULL) {trimwhitespace(buff);}	
			}
			int temp = parseInputLine(buff, n);
			free(buff);
		}
	}
	char* temp = "";
	char* temp2 = "";
	for(int i = 0; i < number_nodes; i++){
		if(n[i].status == -2){
fprintf(stderr, "made it here1\n");
			strcpy(n[i].input, n[i].name);
			strcpy(n[i].output, n[i].name);
			n[i].num_children = 0;
			n[i].status = -1;
			prepend(n[i].output, "Output_");
			strcpy(n[i].prog, candidates);
			prepend(n[i].prog, " ");
			prepend(n[i].prog, n[i].output);
			prepend(n[i].prog, " ");
			prepend(n[i].prog, n[i].input);
			prepend(n[i].prog, "./leafcounter ");
printf("leaf node %s input is %s and output is %s and prog is %s\n", n[i].name, n[i].input, n[i].output, n[i].prog);
fprintf(stderr, "made it here2\n");
		}
		//else if(strcmp(n[i].name, "Who_Won") == 0){
		else{
fprintf(stderr, "made it here3\n");
			char* temp3 = malloc(sizeof(char) * MAX_LINE);
			//prepend(n[i].prog, candidates);
			strcpy(temp3, candidates);
			//prepend(temp3, candidates);
printf("temp3 is %s", temp3);
			char c = number_candidates + '0';
			prepend(temp3, " ");
			prepend(temp3, n[i].output);
			prepend(temp3, " ");
			for(int k = n[i].num_children - 1; k >= 0; k--){
				prepend(temp3, n[i].input[k]);
				prepend(temp3, " "); 
			}
			char buf[999];
			sprintf(buf, "%d", n[i].num_children);
			prepend(temp3, buf);
			if(strcmp(n[i].name, "Who_Won") == 0){
				prepend(temp3, "./find_winner ");
			}
			else{
				prepend(temp3, "./aggregate_votes ");
			}
		strcpy(n[i].prog,temp3);
printf("node %s pros is %s\n", n[i].name, n[i].prog);
		free(temp3);
fprintf(stderr, "made it here4\n");
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
		char* tempi = malloc(sizeof(char)*MAX_LINE);
		tempi = strcpy(tempi, trimwhitespace(s));
		candidates = tempi;		
		line_count++;
		return 0;
	}
	else if(line_count == 2){
		char** strings = malloc(sizeof(char*) * MAX_NODES);
		number_nodes = makeargv(trimwhitespace(s), " ", &strings);
		if(number_nodes == 0){
			return 0;
		}
		for(int i = 0; i < number_nodes; i++){
			strcpy(n[i].name, trimwhitespace(strings[i]));
			n[i].id = i+1;
			n[i].status = -2;
		}
		line_count++;
		return 0;		
	}
	else{
		char** strings2 = malloc(sizeof(char*) * MAX_NODES);
		char** childr = malloc(sizeof(char*) * MAX_NODES);
		char* parent_name = "";
		int numPart = makeargv(trimwhitespace(s), ":", &strings2);
		parent_name = trimwhitespace(strings2[0]);
		int num_child = 0;
		num_child = makeargv(trimwhitespace(strings2[1]), " ", &childr);
		node_t* parent = findnode(n, parent_name); 
		parent->num_children = num_child;
		char* temp = malloc(sizeof(char)*MAX_LINE);
		for(int i = 0; parent_name[i] != '\0'; i++){
			temp[i] = parent_name[i];
		}
		prepend(temp, "Output_");
		strcpy(parent->output, temp);
		parent->status = 1;
printf("output of %s is %s\n", parent->name, parent->output);
		for(int i = 0; i < num_child; i++){
			node_t* child = findnode(n, trimwhitespace(childr[i]));
			parent->children[i] = child->id;
printf("child of %s is %s\n", parent->name, child->name);
			char* temp2 = malloc(sizeof(char)*MAX_LINE);
			strcpy(temp2, child->name);
			char* prepend_name = prepend(temp2, "Output_");
			strcpy(parent->input[i], prepend_name);
printf("input of %s is %s\n", parent->name, parent->input[i]);
		}
		free(strings2);
		free(childr);
		free(temp);
		return num_child;
	}
		
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
		printf("\nleaf name: %s and prog is %s \n", n->name, n->prog);
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
	int numArgs2 = makeargv(trimwhitespace(n->prog), " ", &progArray2);
	progArray2[numArgs2] = NULL; 
	if(n->id == 1){
		printf("\nroot name: %s and prog is %s. \n", n->name, n->prog);
		execv("find_winner", progArray2);
		fprintf(stderr, "%s failed to exec find_winner\n,", n->name);
		exit(1);
	}
	else{
		printf("\nparent name: %s and prog is %s. \n", n->name, n->prog);
		execv("aggregate_votes", progArray2);
		fprintf(stderr, "%s failed to exec aggregate_vote\n,", n->name);
		exit(1);	
	} 
}




/*status
-2:not initizalized
-1: leaf nodes
 1: non-leaf
*/


int main(int argc, char **argv){

	//Allocate space for MAX_NODES to node pointer
	struct node* mainnodes=(struct node*)malloc(sizeof(struct node)*MAX_NODES);

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
