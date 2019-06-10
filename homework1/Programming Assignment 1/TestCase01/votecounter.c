/* 
Login: stein936, yang4055
Date: 02/21/2018
Name: Andrew Steinbrueck, Xin Yang
Student ID: 3949010, 5064064

*/



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

//global variables
//keep track of line number
int lineNumber = -1;
//record the first line to use in the exec calls
char * candidates = "";
//record number of nodes
int numNodes=0;
//record number of region
int numRegion = 0;


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

int parseInput(char *filename, node_t *n){	//parses "input.txt" file, line by line using buffer, assign prog command for each node
	//open the file
	FILE* file = file_open(filename);
	//read the line
	char* buf = malloc(sizeof(char)*MAX_LINE);

	//call parseInputLine if there is new line read
	while ((buf = read_line(buf,file)) > 0){
		
		if (buf[0] != '#'){	//avoid lines starting with '#'
			printf("buff: %s\n", buf);
			parseInputLine(buf, n);		//call parseInputLine
		}
	}

	if(lineNumber+1 < 3){	//for incorrect input, no children	
		fprintf(stderr,"No parent child relationship\n");
		exit(1);
	}

	//fill in the information for leafnode
	int k;
	
	for (k = 0; k< numNodes; k++){
		if(n[k].status == -2){				//leaf nodes
			strcpy(n[k].input[0],n[k].name);	//copy nodes name to node input and output
			strcpy(n[k].output,n[k].name);
			prepend(n[k].output,"Output_");		//add "Output_" to node output
			n[k].num_children = 0;			//leaf nodes have 0 children
			n[k].status = -1;			//now intialized node, change to -1
			
			//assign prog command for each node, leaf nodes call ./leafcounter
			snprintf(n[k].prog, 1024,"%s%s%s%s%s%s","./leafcounter ",		
			trimwhitespace(n[k].input[0])," ",trimwhitespace(n[k].output)," ",
			trimwhitespace(candidates));
		
		}
		if(n[k].status == 1 && k > 0){	//non root, parent nodes (non leave nodes)
			int m;
			char *inputs = malloc(1024);
			for (m=0; m<n[k].num_children; m++){	//concatenates input
				strcat(inputs,n[k].input[m]);		
			}
			
			//convert number of children to string
			char nchild[100];
			sprintf(nchild, "%d", n[k].num_children);

			//assign prog command, parent nodes (non root) call "./aggregate_votes"
			snprintf(n[k].prog, 1024,"%s%s%s%s%s%s%s%s","./aggregate_votes ",
			trimwhitespace(nchild)," ",
			trimwhitespace(inputs)," ",trimwhitespace(n[k].output)," ",
			trimwhitespace(candidates));
				
		}else if(n[k].status ==1 && k ==0){	//root node
			int m;
			char *inputs = malloc(1024);
			
			for (m=0; m<n[k].num_children; m++){
				strcat(inputs,n[k].input[m]);		
			}

			//convert number of children to string
			char nchild[100];
			sprintf(nchild, "%d", n[k].num_children);

			//assign prog command, root node calls "./find_winnter"
			snprintf(n[k].prog, 1024,"%s%s%s%s%s%s%s%s","./find_winner ",
			trimwhitespace(nchild)," ",
			trimwhitespace(inputs)," ",trimwhitespace(n[k].output)," ",
			trimwhitespace(candidates));
		
		}
	}

	return numNodes;	//total number of nodes in election tree
		
	
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
int parseInputLine(char *s, node_t *n){		//parses each string (excluding white spaces) of each line
	lineNumber++;	//read line, increment lineNumber of file
	
	if (lineNumber == 0){	//first line of text file, determines number of candiates
		char * tempCandi = malloc(sizeof(char)*1024);	//allocate string
		tempCandi = strcpy(tempCandi, trimwhitespace(s));	//copy string from first line into temp string
		candidates=tempCandi;					//candidate names stored as string
		return 0;
	}
	
	char **strArray = malloc(sizeof(char *)*MAX_NODES);	//array to hold strings from each txt file line
	if (lineNumber == 1){					//second line of code in file
		numNodes = makeargv(trimwhitespace(s), " ",&strArray);	//determines total number of nodes
		if (numNodes < 2){					//for test cases with only 1 node total
			fprintf(stderr, "No child node\n");
			exit(1);
		}
		
		int i;
		//specify the name/id for each node;
		for (i = 0; i<numNodes;i++){				//goes through each node and assigns string name, id, and status
			strcpy(n[i].name, trimwhitespace(strArray[i]));
			n[i].id = i+1;
			n[i].status = -2;		
		}
		return 0;
	}
	
	//for all line numbers 3 or greater (>)

	char **childArray = malloc(sizeof(char *)*MAX_NODES);	//holds name of children as strings
	char *parentName;					//parent name string
	int numChild;						//parent's number of children 
	int numPart = makeargv(trimwhitespace(s), ":", &strArray); //first parse out ':'
	if (numPart != 2){						//for incorrect input, too many ':' or no ':'
		fprintf(stderr, "skip line for lack of info\n");
		lineNumber=lineNumber-1;	//go back to previous line
		return numRegion;
	}else{	//for correct input
		parentName = trimwhitespace(strArray[0]);	//first string before ':' is parent name
		numChild = makeargv(trimwhitespace(strArray[1]), " ", &childArray);	//parse through all children names using ' ' delimiter
		if(lineNumber == 3){		//return number of regions on third line
			numRegion = numChild;
		}
		
		//find parent node
		node_t* parentNode = malloc(sizeof(node_t));	//allocate node
		parentNode = findnode(n, parentName);		//go through nodes and finds node that matches parent name

		parentNode -> num_children = numChild;		//initialize number of children for correct parent node 
		char* prepend_name = malloc(sizeof(char)*1024);	//allocate string
		prepend_name = parentName;			//set string to parent node name
		prepend(prepend_name,"Output_");		//add string "Output_" to temp string
		strcpy(parentNode -> output, prepend_name);	//copy parent node output to have temp string 
	
		parentNode -> status = 1;	//all parent nodes (intialized) have status 1
		//find each child
		int j;
		for (j=0; j < numChild; j++){
			node_t* childNode = malloc(sizeof(node_t));		//allocate children nodes
			childNode = findnode(n, trimwhitespace(childArray[j]));	//find's children nodes by comparing with children names
			parentNode -> children[j] = childNode -> id;		//initialize these children of parent to have correct id's
			snprintf(parentNode->input[j],1024,"%s%s"," Output_",trimwhitespace(childArray[j]));//put child output file name
		}											    //as parent input
		return numRegion;	
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
void execNodes(node_t *n){	//executes each process, starting with leaf non parent processs, adds up total votes and determines winner
	if (n->num_children == 0){			//for leaf nodes
		printf("leafnode: %s\n", n->name);
		
		char **progArray = malloc(sizeof(char *)*MAX_NODES);
		int numarg = makeargv(trimwhitespace(n->prog), " ", &progArray);  //make prog into array of string
		progArray[numarg] = NULL;	//end of string is null

		execv("leafcounter", progArray);	//leaf nodes execute, no waiting
		fprintf(stderr, "%s faild to exec leafcounter\n", n->name);	//for failed exec
		exit(1);
	}else if(n->num_children > 0){	//for parent nodes
		int temp;
		pid_t pids;
		for (temp=0; temp < n->num_children; temp++){
			pids = fork();		//forks for each child of parent node
			if(pids == -1){		//failed fork rror
				fprintf(stderr,"Fail to fork for parent: %s\n", n->name);
				exit(1);			
			}else if(pids == 0){		//child node
				node_t *chld = findNodeByID(n,n->children[temp]);//find child node, get child pid, and then recursive call for child
				chld->pid = getpid();
				execNodes(chld);
				break;
			}else if(pids > 0){		//parent node
				pid_t waitpid;
				while(((waitpid = wait(NULL)) == -1) && (errno == EINTR));  //wait for children nodes to execute
				if (pids != waitpid){	//fail to wait error
					fprintf(stderr, "Fail to wait due to error");
					exit(1);
				}
			}
			
		}
		
		if(pids > 0){	//for parent nodes waiting after all children finish executing, 
			char **progArray = malloc(sizeof(char *)*MAX_NODES);
			int numarg = makeargv(trimwhitespace(n->prog), " ", &progArray);	//make into array
			progArray[numarg] = NULL;
			if (n->id == 1){		//for root, execute find_winner
				printf("winner\n");
				execv("find_winner",progArray);	
				fprintf(stderr, "%s faild to exec find_winner\n", n->name);
				exit(1);		
			}else{				//for parent non root, execute aggregate voteeees
				printf("aggregate: %s\n", n->name);
				execv("aggregate_votes", progArray);
				fprintf(stderr, "%s faild to exec aggregate_votes\n", n->name);
				exit(1);
			}	
		}
		

	}	
}




/*status
-2:not initizalized
-1: leaf nodes
 1: non-leaf
*/




int main(int argc, char **argv){	//main driver

	//Allocate space for MAX_NODES to node pointer
	struct node* mainnodes=(struct node*)malloc(sizeof(struct node)*MAX_NODES);

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




















