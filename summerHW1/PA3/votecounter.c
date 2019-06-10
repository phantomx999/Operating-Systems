#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include "util.h"
#include <sys/types.h>
#include <sys/stat.h>

extern int errno;

#define MAX_NODES 100
#define MAX_LINE 1024

char* candidate_names[MAX_NODES];
//int number_of_candidates = 0;
int leaf = 0;
char global_arg[MAX_LINE]; 

typedef struct candidate{
	char* name[MAX_LINE];
	int count;
} candidates;

candidates* findCandidate(candidates* start, char* tobefound){
	//Find the node in question
		candidates* temp = start;
		while(strcmp(temp->name, "") != 0){
			if( (strcmp(temp->name, tobefound)==0)){
				return temp;
			}
			temp++;
		}
		return NULL;
}

struct node* findNode(struct node** start, char* tobefound){
	//Find the node in question
		struct node* temp = *start;
		while(temp != NULL){
			if(temp->name == NULL) return NULL;
			if( (strcmp(temp->name, tobefound)==0)){
				return temp;
			}
			temp = temp->next;
		}
		return NULL;
}

int readline(int fd, char *buf, int nread){
	int numread = 0;
	int returnval;
	while(numread < (nread-1)){
		returnval = read(fd, buf+numread, 1);
		if ((returnval == -1) && (errno == EINTR)){
			continue;
		}
		if((returnval ==0) && (numread == 0)){
			return 0;
		}
		if(returnval == 0) break;
		if(returnval == -1) return -1;
		numread++;
		if(buf[numread-1] == '\n'){
			buf[numread] = '\0';
			return numread; 
		}		
	}
	errno = EINVAL;
	return -1;
}

int readInput(char* path, int file_count, struct queue** Q){
	char* temp = malloc(sizeof(char)*strlen(path));
	strcpy(temp, path);
	trimwhitespace(temp);
	temp = dirname(temp);
	//strcat(temp, "/");
	struct queue* q = *Q;
	struct dirent* dir;
	DIR* p_dir;
	p_dir = opendir(trimwhitespace(path));
	if(p_dir == NULL){
		fprintf(stderr, "Cannot open directory1 %s\n", path);
		exit(EXIT_FAILURE);
	}	
	while( (dir = readdir(p_dir)) != NULL){
		if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..") || !strcmp(dir->d_name, ".DS_Store")){
			continue;
		}		
fprintf(stderr, "temp = %s\n", temp);
			file_count++;
			strcat(temp, "/");
			strcat(temp, dir->d_name);
			//snprintf(temp, 1+strlen(dir->d_name), "%s%s", "/", dir->d_name);
			trimwhitespace(temp);
fprintf(stderr, "temp2 = %s\n", temp);
			q->file = malloc(sizeof(char)*strlen(temp));
			strcpy(q->file, trimwhitespace(temp));
			q->next = malloc(sizeof(struct queue));
			q = q->next;
			temp = dirname(temp);
fprintf(stderr, "temp3 = %s\n", temp);
/*
			int votesfile = open(temp, O_RDONLY);
			if(votesfile < 0){
				fprintf(stderr, "could not open %s file to read\n", temp);
				exit(EXIT_FAILURE);
			}
			int dest_file = open(dirname(temp), O_CREAT | O_WRONLY | O_TRUNC, 0777);
			if(dest_file < 0){
				fprintf(stderr, "could not open %s file to read\n", dirname(temp));
				exit(EXIT_FAILURE);
			}
			char* buffer = malloc(sizeof(char)* MAX_LINE);
			int read_line = 0;
			while((numread = readline(votesfile, buffer, MAX_LINE)) != 0){
				read_line = 1;
				int numWrite = write(dest_file, trimwhitespace(buffer), strlen(trimwhitespace(buffer)));
				if(numWrite == -1){
					perror("fail to write the file\n");
					return;
				}
			}
				
			}
			if(read_line == 0){
				fprintf(stderr, "could not read file\n", read_line);
				exit(EXIT_FAILURE);
			}
*/
	}
	q = NULL;
q = *Q;
while(q != NULL){
	fprintf(stderr, "%s\n", q->file);
	q = q->next;
}
	if(file_count == 0){
		perror("no files in input directory\n");
		exit(EXIT_FAILURE);
	}
	return file_count;
}

void makeDirectories(struct node** dummy, char* path){
		struct node* list = *dummy;
		char* temp;
		temp = malloc(sizeof(char)*MAX_LINE);
		strcpy(temp, path);
		trimwhitespace(temp);
		if(list->status == 2){
			mkdir(temp, 0700);			
			return;
		}
		else{			
			for(int i = 0; i < list->number_of_children; i++){
				strcat(temp, "/");				
				strcat(temp, list->children[i]);
				mkdir(temp, 0700);
				struct node* n = findNode(&list, basename(temp));
				if(n == NULL){
					fprintf(stderr, "could not find makeDirectories find node\n");
					return;
				}
				makeDirectories(&n, temp);
				if(i != list->number_of_children){
					temp = dirname(temp);
				}
			}	
		}
		return;
}

void makeOutput(char* arg, struct node **dummy){
	if(*dummy == NULL){
		fprintf(stderr, "error in makeOutput *dummy, no nodes to make output directory\n");
		exit(EXIT_FAILURE);
	}
	char* path = malloc(sizeof(char)*MAX_LINE);
	strcpy(path, arg);
	struct stat st = {0};
	if(stat(path, &st) == -1){
		mkdir(path, 0700);
	}
	else{
		remove(path);
		mkdir(path, 0700);
	}
	strcat(path, "/");
	struct node* current = *dummy;
	if(current == NULL){
		fprintf(stderr, "error in makeOutput, no nodes to make output directory\n");
		exit(EXIT_FAILURE);
	}
	strcat(path, current->name);
	mkdir(path, 0700);
	makeDirectories(&current, path);
}

struct node* makeDAG(char *DAG){
	int numread = 0;
	char* readbuffer = malloc(sizeof(char)* MAX_LINE);
	int read_line = 0;
	struct node *start = (struct node*)malloc(sizeof(struct node));
	start->name = "";
	struct node* current = start;
	struct node* n = NULL;
	int initialized = 0;
	int votesfile = open(DAG, O_RDONLY);
	if(votesfile < 0){
		fprintf(stderr, "could not open %s file to read\n", DAG);
		exit(EXIT_FAILURE);
	}
	while((numread = readline(votesfile, readbuffer, MAX_LINE)) != 0){
		read_line = 1;
		char** tempNodes = malloc(sizeof(char*)*MAX_LINE);
		int temp = makeargv(trimwhitespace(readbuffer), ":", &tempNodes);
		for(int i = 0; i < temp; i++){
			n = findNode(&start, tempNodes[i]); 
			if(n == NULL){ fprintf(stderr, "made it here\n");
				current->name = malloc(sizeof(char)*MAX_LINE);
				strcpy(current->name, tempNodes[i]);
				trimwhitespace(current->name);
				if(i == 0){
					current->number_of_children = temp-1;
					current->children = (char**)malloc(sizeof(char*)*current->number_of_children*MAX_LINE);
					for(int z = 0; z < current->number_of_children; z++){
						current->children[z] = (char*)malloc(sizeof(char)*current->number_of_children);
					}
					for(int j = 0; j < current->number_of_children; j++){
						strcpy(current->children[j], tempNodes[j+1]);
						trimwhitespace(current->children[j]);	
					}
					(initialized == 0)? (current->status = 0):(current->status = 1);
				}
				else{
					current->number_of_children = 0;
					current->children = NULL;
					current->status = 2;
					current->number_of_children = 0;
				}
				initialized = 1;
				current->next = (struct node*)malloc(sizeof(struct node));
				current->next->next = NULL;
				current = current->next; 
			}
			else{
				n->status = 1;
				n->number_of_children = (temp-1);
				n->children = (char**)malloc(sizeof(char*)*n->number_of_children*MAX_LINE);
				for(int z = 0; z < n->number_of_children; z++){
					n->children[z] = (char*)malloc(sizeof(char)*n->number_of_children);
				}
				for(int i = 0; i < n->number_of_children; i++){
					strcpy(n->children[i], tempNodes[i+1]);	
					trimwhitespace(n->children[i]);	
				}
			}
		}
	}
	current = NULL;
	if(read_line == 0){
		fprintf(stderr, "read line = 0, couldnt read %s\n", DAG);
		exit(EXIT_FAILURE);
	}
	return start;
}

int main(int argc, char **argv){	//main driver

	if (argc != 4){		//incorrect number of arguments
		printf("Usage: %s Program\n", argv[0]);
		return -1;
	}

	char* DAG = (argv[1]);
	struct node *front = NULL;	
	front = makeDAG(DAG);
//displayName(&front);
	makeOutput(argv[3], &front);
	struct queue* Q = malloc(sizeof(struct queue));
	int number_of_files = readInput(argv[2], 0, &Q);
struct queue* q = Q;
while(q != NULL){
	fprintf(stderr, "%s\n", q->file);
	q = q->next;
}
	return 0;

}

