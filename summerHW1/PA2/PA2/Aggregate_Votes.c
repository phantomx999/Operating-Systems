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
//int number_of_candidates = 0;
int leaf = 0;
char global_arg[MAX_LINE]; 

typedef struct candidate{
	char* name[MAX_LINE];
	int count;
} candidates;

candidates* findnode(candidates*, char*);
int readLine(int, char*, int);
char* traverse_directory2(char* arg);

candidates* findnode(candidates* start, char* tobefound){
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

int find_old_file(char*arg, DIR* p_dir);
int find_old_file(char*arg, DIR* p_dir){
	char* temp = malloc(sizeof(char)*MAX_LINE);
	int ret;
	strcat(temp, get_last_directory(arg));
	strcat(temp, ".txt");
	struct dirent* dir;
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

char* traverse_directory2(char* arg){
	struct dirent* dir;
	DIR* p_dir;
	p_dir = opendir(arg);
	char path[MAX_LINE];
	path[0] = '\0';
	strcat(path, trimwhitespace(arg));
	if(p_dir == NULL){
		fprintf(stderr, "Cannot open directory1 %s\n", arg);
		exit(EXIT_FAILURE);
	}
	int old_file_exists = find_old_file(arg, p_dir);
	rewinddir(p_dir);
	while( (dir = readdir(p_dir)) != NULL){
		if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")){
			continue;
		}
		if(dir->d_type != DT_DIR && strcmp(dir->d_name, "votes.txt") == 0){
			path[strlen(path)] = NULL;
			char **dummy = malloc(sizeof(char*)*MAX_LINE);
			int dumdum = makeargv(trimwhitespace(path), " ", &dummy);
			dummy[dumdum] = NULL;
			execv("Leaf_Counter", dummy);	//   Leaf_Counter,  
			fprintf(stderr, "%s failed to exec Leaf_Counter\n", path);
			exit(EXIT_FAILURE);
		}
	}
	rewinddir(p_dir);
	while( (dir = readdir(p_dir)) != NULL){
		if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..") || dir->d_type == DT_REG){
			continue;
		}
		else{
			pid_t pid;				
			if( (pid = fork()) == -1){
				fprintf(stderr, "Fail to fork with child: %s\n", path);
				exit(1);
			}
			if(pid == 0){	//child
				strcat(path, "/");
				strcat(path, dir->d_name);
//fprintf(stderr, "pid 0 path = %s\n", path);
				traverse_directory2(trimwhitespace(path));
			}
			else{	//parent
				pid_t waitpid;
				while((waitpid = wait(NULL) == -1) && (errno == EINTR)){
					if(pid != waitpid){
						fprintf(stderr, "Fail to wait due to error");
						exit(1);
					}
				}
			}		
		}
	}
	closedir(p_dir);
	struct candidate* n = malloc(sizeof(struct candidate)*MAX_NODES);
	int number_of_candidates = 0;
//fprintf(stderr, "arg = %s\n", arg);
//fprintf(stderr, "path = %s\n", path);
	trimwhitespace(path);
	//fprintf(stderr, "path = %s\n", path);
	p_dir = opendir(path);
//	char* temp = malloc(sizeof(char)*MAX_LINE);
//	strcpy(temp, path);
	//fprintf(stderr, "temp = %s\n", temp);
	while(((dir = readdir(p_dir)) != NULL) ){
		if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..") || !strcmp(dir->d_name, "votes.txt")){
			continue;
		}
		if(dir->d_type == DT_DIR){
			char* temp = malloc(sizeof(char)*MAX_LINE);
			strcpy(temp, path);
			strcat(temp, "/");
			//fprintf(stderr, "temp = %s\n", temp);
			strcat(temp, dir->d_name);
			char* new_path = malloc(sizeof(char)*MAX_LINE);
			strcpy(new_path, temp);
			strcat(new_path, "/");
			fprintf(stderr, "newpath_b4 = %s\n", new_path);
			strcat(new_path, dir->d_name);
			fprintf(stderr, "newpath_after = %s\n", new_path);
			strcat(new_path,".txt");
			int votesfile = open(new_path, O_RDONLY);
			if(votesfile < 0){
				fprintf(stderr, "could not open %s file to read\n", new_path);
				exit(EXIT_FAILURE);
			}
			int numread = 0;
			//new_path = "";
			fprintf(stderr, "arg2 = %s\n", arg);
			fprintf(stderr, "path2 = %s\n", path);
			//temp = path;
			//temp = "";
			//strcpy(temp, path);
			//fprintf(stderr, "temp2 = %s\n", temp);
			char* readbuffer = malloc(sizeof(char)* MAX_LINE);
			int read_line = 0;
			while((numread = readline(votesfile, readbuffer, MAX_LINE)) != 0){
				read_line = 1;				
				char **cansArray = malloc(sizeof(char*) * MAX_NODES);
				int numCans = makeargv((trimwhitespace(readbuffer)), ",", &cansArray);
				for(int i = 0; i < numCans; i++){
					char** votesArray = malloc(sizeof(char*)*3);
					int num = makeargv(trimwhitespace(cansArray[i]), ":", &votesArray);
					int numVotes = atoi(votesArray[1]);
					candidates* cnode = findnode(n, votesArray[0]);
					if(cnode == NULL){
						strcpy(n[number_of_candidates].name, votesArray[0]);
						n[number_of_candidates].count = numVotes;
						number_of_candidates++;
					}
					else{
						cnode->count = cnode->count + numVotes;  
					}
				} 
			}
			if(read_line == 0) fprintf(stderr, "Failed to read\n");
			close(votesfile);
		}
	}
	closedir(p_dir);
  			
	if (number_of_candidates == 0) fprintf(stderr, "Nothing to write!\n");

	if (number_of_candidates > 0){
		char *writePath = malloc(sizeof(char)* MAX_LINE);
		char *base = basename(strdup(arg));
		char* last_directory = get_last_directory(arg);
		snprintf(writePath, MAX_LINE, "%s%s%s%s", trimwhitespace(arg), "/", base, ".txt");
//fprintf(stderr, "writePath is %s\n", writePath);
		int writefile = open(writePath, O_CREAT | O_RDWR | O_TRUNC, 0777);
//fprintf(stderr, "writePath is %s\n", writePath);
		int i;
		char *canInfo = malloc(sizeof(char)*MAX_LINE);
		for(i = 0; i < number_of_candidates; i++){
			char nVotes[255];
			sprintf(nVotes, "%d", n[i].count);
			if(i == (number_of_candidates -1)){
				snprintf(canInfo, MAX_LINE, "%s%s%s", n[i].name, ":", nVotes);
			}
			else{
				snprintf(canInfo, MAX_LINE, "%s%s%s%s", n[i].name, ":", nVotes, ",");
			}
			int numWrite = write(writefile, trimwhitespace(canInfo), strlen(trimwhitespace(canInfo)));
			if(numWrite == -1){
				perror("fail to write the file\n");
				return;
			}
		}
		int numWrite = write(writefile, "\n", 1);
		close(writefile);	
	}	
}

int main(int argc, char** argv){
	if(argc == 1){
		strcpy(global_arg, argv[0]);
		trimwhitespace(global_arg);
		char* temp = malloc(sizeof(char)*MAX_LINE);
		temp = get_last_directory(argv[0]);
		char* print_output = malloc(sizeof(char)*MAX_LINE);
		strcpy(print_output, argv[0]);
		strcat(print_output, "/");
		strcat(print_output, temp);
		strcat(print_output, ".txt");	
		traverse_directory2(argv[0]);
		//printf("%s\n", print_output);
	}
	else if(argc != 2){
		printf("Usage: %s Program\n", argv[0]);
		return -1;
	}
	else{
		strcpy(global_arg, argv[1]);
		trimwhitespace(global_arg);
		char* temp = malloc(sizeof(char)*MAX_LINE);
		temp = get_last_directory(argv[1]);
		char* print_output = malloc(sizeof(char)*MAX_LINE);
		strcpy(print_output, argv[1]);
		strcat(print_output, "/");
		strcat(print_output, temp);
		strcat(print_output, ".txt");	
		traverse_directory2(argv[1]);
		printf("%s\n", print_output);
	}
	return 0;
}
