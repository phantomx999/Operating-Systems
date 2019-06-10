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
int from_execv = 0;

typedef struct candidate{
	char* name[MAX_LINE];
	int count;
} candidates;

char* get_last_directory(char* s){
	char c = ' ';
	int i = 0;
	int count = 0;
	char* reverse_string = malloc(sizeof(char)*strlen(s));
	for(i = (strlen(s) - 1); i >= 0; i--){
		c = s[i];
		if(c == '/'){
			break;
		}
		reverse_string[count] = s[i];
		count++;
	}
	char* new_string = malloc(sizeof(char)*strlen(reverse_string));
	count = 0;
	for(int j = strlen(reverse_string) - 1; j >= 0; j--){
		new_string[count] = reverse_string[j];
		count++;
	}
	return new_string;
}

int find_old_file(char*arg, DIR* p_dir);
int find_old_file(char*arg, DIR* p_dir){
	char* temp = malloc(sizeof(char)*MAX_LINE);
	int ret;
	strcat(temp, get_last_directory(arg));
	strcat(temp, ".txt");

	struct dirent* dir;
	//DIR* p_dir;
	//p_dir = opendir(arg);
	if(p_dir == NULL){
		fprintf(stderr, "Cannot open directory %s\n", arg);
		exit(EXIT_FAILURE);
	}
	while( (dir = readdir(p_dir)) != NULL){
		if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")){
			continue;
		}
		if(!strcmp(temp, dir->d_name)){
			char* old_file = malloc(sizeof(char)*MAX_LINE);
			char cwd[MAX_LINE];
			if(getcwd(cwd, sizeof(cwd)) == NULL){
				perror("getcwd() error");
			}
			strcpy(old_file, cwd);
			strcat(old_file, "/"); 
			strcat(old_file, arg);
			strcat(old_file, "/");
			strcat(old_file, dir->d_name); 
			ret = remove(old_file);
			if(ret != 0) perror("did not delete old file\n");
		} 	
	}
	return 0;
}

char* traverse_directory(candidates* n, char* arg);
char* traverse_directory(candidates* n, char* arg){
	char return_array[MAX_LINE];
	struct dirent* dir;
	DIR* p_dir;
	p_dir = opendir(arg);
	if(p_dir == NULL){
		fprintf(stderr, "Cannot open directory %s\n", arg);
		exit(EXIT_FAILURE);
	}
	int old_file_exists = find_old_file(arg, p_dir);
	rewinddir(p_dir);
	while( (dir = readdir(p_dir)) != NULL){
		if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")){
			continue;
		}
		char path[strlen(trimwhitespace(arg)) + strlen(4+dir->d_name)];
		//path[0] = '\0';
		//strcat(path, trimwhitespace(arg));
		//if(strcmp(path,arg) == 0){
			if(dir->d_type != DT_DIR && !strcmp(dir->d_name, "votes.txt")){
				char path[strlen(trimwhitespace(arg)) + strlen(4+dir->d_name)];
				path[0] = '\0';
				strcat(path, trimwhitespace(arg));
				strcat(path, "/");
				strcat(path, dir->d_name);
				FILE* f = fopen(path, "r");
				if(f == NULL){
					perror("Could not open file of path\n");
					return EXIT_FAILURE;
				}
				count_leaf(f, n);
				char* output_text_file_path = malloc(sizeof(char)*MAX_LINE);
				char* new_file = malloc(sizeof(char)*MAX_LINE);
				strcpy(output_text_file_path, arg);
				char* temp = get_last_directory(arg);
				strcat(output_text_file_path, "/");
				strcat(output_text_file_path, temp);
				strcat(output_text_file_path, ".txt");
				strcpy(new_file, temp);
				strcat(new_file, ".txt");
				char* full_path = malloc(sizeof(char)*MAX_LINE);
				char cwd[MAX_LINE];
				if(getcwd(cwd, sizeof(cwd)) == NULL){
					perror("getcwd() error");
				}
				strcpy(full_path, cwd);
				strcat(full_path, "/"); 
				strcat(full_path, arg);
				strcat(full_path, "/");
				strcat(full_path, new_file);
				trimwhitespace(output_text_file_path);
				if(!from_execv){
					printf("%s\n", output_text_file_path);
				}
				char* output_t = malloc(sizeof(char)*MAX_LINE);
				output_t[0] = '\0';
				for(int i = 0; i < (number_of_candidates*4); i++){
					strcat(output_t, n[i].name);
					strcat(output_t, ":");
					char str[3];
					sprintf(str, "%d", n[i].count);
					strcat(output_t, str);
					strcat(output_t, ",");
				}
				output_t[(number_of_candidates*4)] = '\0';
				trimwhitespace(output_t);
				fclose(f);
				FILE* f2 = fopen(full_path, "w");
				if(f2 == NULL){
					perror("file f2 could not be opened or it is empty");
					return EXIT_FAILURE;
				}
				if(fputs(output_t, f2) < 0){
					perror("Couldnt write to file");
					return EXIT_FAILURE;
				}
				fclose(f2);
				closedir(p_dir);
				return path;
			}
			else{
				printf("Not a leaf node.\n");
				return;
			}
		//}
	}
	fprintf(stderr, "No directory path matches argument %s\n", arg);
	return EXIT_FAILURE;
}

void count_leaf(FILE* f, candidates* n){
	char* buffer = malloc(sizeof(char)* MAX_LINE);
	int read_file = 0;
	while(fgets(trimwhitespace(buffer), sizeof(buffer), f) != NULL && buffer > 0){
		read_file = 1;
		int name_not_found = 1;
		trimwhitespace(buffer);
		for(int i = 0; i < number_of_candidates; i++){
			if(strcmp(n[i].name, buffer) == 0){
				n[i].count++;
				name_not_found = 0;
			}
		}
		if(name_not_found){
			strcpy(n[number_of_candidates].name, trimwhitespace(buffer));
			n[number_of_candidates].count++;
			number_of_candidates++;
		}
	}
	if(!read_file) exit(EXIT_FAILURE);
	return;
}

int main(int argc, char **argv){
	struct candidates* mainnodes = (struct candidates*)malloc(sizeof(candidates)*MAX_NODES);	
	char* output;	
	if (argc == 1){
		from_execv = 1;
		output = traverse_directory(mainnodes, argv[0]);
	}				
	else if(argc != 2){
		printf("Usage: %s Program\n", argv[0]);
		return -1;
	}
	else{
		output = traverse_directory(mainnodes, argv[1]);
	}
	if(output == EXIT_FAILURE){
		fprintf(stderr, "Leaf Counter Failed.  Terminating Program");
		return -1;
	}
	return 0;
}
