#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include "util.h"

extern int errno;

#define MAX_NODES 100
#define MAX_LINE 1024

char* candidate_names[MAX_NODES];
int number_of_candidates = 0;

typedef struct candidate{
	char* name[MAX_LINE];
	int count;
} candidates;

void print_output(char* s){
	fprintf(stderr, "made it here print\n");
	for(int i = 0; i < strlen(s); i++){
		printf("%c", s[i]);
	}
	fprintf(stderr, "s is %s\n", s);
	printf("\n");
}

char* get_last_directory(char* s){
	char c = ' ';
	int i = 0;
	int count = 0;
	char* reverse_string = malloc(sizeof(char) * strlen(s));
	for(i = (strlen(s) - 1); i >= 0; i--){
		c = s[i];
		if(c == '/'){
			break;
		}
		reverse_string[count] = s[i];
		count++;
	}
	char* new_string = malloc(sizeof(char) * strlen(reverse_string));
	count = 0;
	for(int j = strlen(reverse_string) - 1; j >= 0; j--){
		new_string[count] = reverse_string[j];
		count++;
	}
	return new_string;
}

char* traverse_directory(candidates* n, char* arg){
	char return_array[MAX_LINE];
	struct dirent * dir;
	DIR * p_dir;
	p_dir = opendir(arg);
	if(p_dir == NULL){
		fprintf(stderr, "Cannot open directory %s\n", arg);
		exit(EXIT_FAILURE);
	}
	while((dir = readdir(p_dir)) != NULL){	
		if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")){			
			continue;
		}
		char path[strlen(arg) + strlen(dir->d_name + 4)];
		path[0] = '\0';
		strcat(path, trimwhitespace(arg));
fprintf(stderr, " arg is %s and path is %s\n", arg, path);				
		if(strcmp(path,arg) == 0){
			if(dir->d_type != DT_DIR){
				strcat(path, "/");
				strcat(path, dir->d_name);
				fprintf(stderr, " d_name = %s and path is %s\n", dir->d_name, path);				
				FILE* f =  fopen(path, "r");
				if(f == NULL){
					perror("Could not open file");
					return EXIT_FAILURE;
				}							
				Leaf_Counter(f, n);
				//char output_text_file[strlen(dir->d_name + 4)];
				char output_text_file[strlen(arg)];
				//char temp[strlen(path)];
				char* temp = get_last_directory(arg);
				output_text_file[0] = '\0';
				strcat(output_text_file, temp);
				//strcat(output_text_file, trimwhitespace(path));				
				//strcat(output_text_file, trimwhitespace(dir->d_name));
				strcat(output_text_file, ".txt");
				fprintf(stderr, "output_text_file is %s\n", output_text_file);
				char output_text[100] = "";
				int i = 0;
				printf("here is candidates 0 name: %s\n", n[0].name);
				printf("here is candidates 0 count: %d\n", n[0].count);
				for(i = 0; i < number_of_candidates; i++){
					strcat(output_text, n[i].name);					
					strcat(output_text, ":");
					char str[3];
					sprintf(str,"%d", n[i].count);					
					strcat(output_text, str);
					strcat(output_text, ","); 
				}
				//
				output_text[(number_of_candidates * 4) - 1] = '\0';
				trimwhitespace(output_text);
				fprintf(stderr, "made it here2....\n");
 				for(int i = 0; i < strlen(output_text); i++){
					printf("%c\n", output_text[i]);
				}
				fprintf(stderr, "made it here3\n");
				//
				fclose(f);
				output_text[number_of_candidates * 4] = '\0';
				FILE* f2 = fopen(output_text_file, "w+");
				if(f2 == NULL){
					perror("Could not open file");
					return EXIT_FAILURE;
				}
				if(fputs(output_text, f2) < 0){			
					fprintf(stderr, "Trouble writing to output file\n");
					return EXIT_FAILURE;
				}
		fprintf(stderr, "path is %s\n", path);		
				return_array[0] = '\0';
				strcat(return_array,path);
		fprintf(stderr, "return_array is %s\n", return_array);	
				print_output(path);
				//fclose(f);
				char cwd[MAX_LINE];
				if(getcwd(cwd, sizeof(cwd)) == NULL){
					perror("getcwd() error");
				} 
				char final_dir[MAX_LINE];
				final_dir[0] = '\0';
				strcat(final_dir, cwd);
				strcat(final_dir, "/");
				strcat(final_dir, arg);
				strcat(final_dir, "/");
				strcat(final_dir, output_text_file);
				fprintf(stderr, "cwd is %s and arg is %s and final_dir is %s\n", cwd, arg, final_dir);		
				rename(output_text_file, final_dir);	
				fclose(f2);
				return path;
			}
			else{
				fprintf(stderr, "Not a leaf directory\n");
				if(				
				pid_t child = fork()
				if(child == 0){
					Aggregate_Votes(n, path);
				}
				else if(child > 0){
					waitpid(NULL);
					aggregate votes();
				}
				else{
					perror("fork failed");
					return EXIT_FAILURE;
				}
				strcat(path, d_name);
				//return EXIT_FAILURE;
			}	
		}
		else{			
				fprintf(stderr, "Path does not match argument %s\n", arg); return EXIT_FAILURE;																	
		}	
	}
		fprintf(stderr, "return array is %s\n", return_array);		
	return return_array;
}

void Leaf_Counter(FILE* f, candidates* n){
	char* buffer = malloc(sizeof(char) * MAX_LINE);
	while(fgets(trimwhitespace(buffer), sizeof(buffer), f) != NULL && buffer > 0){						
		int name_not_found = 1;
		trimwhitespace(buffer);
		for(int i = 0; i < number_of_candidates; i++){
			if (strcmp(n[i].name, buffer) == 0){
				n[i].count++;
				name_not_found = 0;
			}
		}
		if(name_not_found == 1){
			strcpy(n[number_of_candidates].name,trimwhitespace(buffer));
			n[number_of_candidates].count++;
			number_of_candidates++;		
		}
	}
///
	for(int i = 0; i < number_of_candidates; i++){
		fprintf(stderr, "n[%d].name is %s and %d\n", i, n[i].name, n[i].count);		
	}
////	
	return;
}

void Aggregate_Votes(candidates* mainnodes, char* arg){
	char* output = traverse_directory(mainnodes, arg);

	if(output == EXIT_FAILURE){
		fprintf(stderr, "Failed. Terminating program\n");
		return -1;
	}
	fprintf(stderr, "output is %s\n", output);
	//print_output(output);
	fprintf(stderr, "made it here final\n");
	//printgraph(mainnodes,numNodes);			//print tree (DAG)
}

int main(int argc, char **argv){	//main driver

	//Allocate space for MAX_NODES to node pointer
	//struct node* mainnodes=(struct node*)malloc(sizeof(struct node)*MAX_NODES);
	struct candidates* mainnodes = (struct candidates*)malloc(sizeof(candidates) * MAX_NODES);

	if (argc != 2){		//incorrect number of arguments
		printf("Usage: %s Program\n", argv[0]);
		return -1;
	}
	
	Aggregate_Votes(mainnodes, argv[1]);

	return 0;
}
