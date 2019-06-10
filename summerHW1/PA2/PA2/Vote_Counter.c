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

int findMax(struct candidate* cans, int len);
char *findWinner(struct candidate* cans, int len);

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
/*
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
*/

int findMax(struct candidate* cans, int len){
	int max = cans[0].count;	
	for(int i = 1; i < len; i++){
		if(max < cans[i].count) max = cans[i].count;
	}
	return max;
}

char *findName(struct candidate* cans, int len){
	char *winner = malloc(sizeof(char)*MAX_LINE);
	int max = findMax(cans, len);
	for(int i = 0; i<len; i++){
		if(max == cans[i].count) {
			strcpy(winner, cans[i].name);
			return trimwhitespace(winner);
		}
	}
	fprintf(stderr, "error in findWinner, could not find winner name\n");
	exit(EXIT_FAILURE);
}

int who_won(char* arg){
	char *path = malloc(sizeof(char)*MAX_LINE);
	strcpy(path, arg);
	strcat(path, "/");
	strcat(path, basename(strdup(arg)));
	strcat(path, ".txt");
	remove(path);	//remove any existing file so that it can be replaced

	//int old_file = open(path, O_CREAT | O_RDWR | O_TRUNC, 0777);
	//close(old_file);
/*
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
*/
	pid_t pid;				
	if( (pid = fork()) == -1){
		fprintf(stderr, "Fail to fork with child: %s\n", path);
		exit(1);
	}
	if(pid == 0){	//child
		arg[strlen(arg)] = NULL;
		char **dummy = malloc(sizeof(char*) * MAX_LINE);
		int dumdum = makeargv(trimwhitespace(arg), " ", &dummy);
		fprintf(stderr, "dumdum = %d\n", dumdum);
		dummy[dumdum] = NULL;
		execv("Aggregate_Votes", dummy);
		fprintf(stderr, "%s failed to exec Leaf_Counter\n", path);
		exit(EXIT_FAILURE);		
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
	//int votesfile = open(path, O_RDWR);
	int votesfile = open(path, O_RDWR);
	if(votesfile < 0){
		fprintf(stderr, "could not open %s file to read\n", path);
		exit(EXIT_FAILURE);
	}
	int numread = 0;
	char* readbuffer = malloc(sizeof(char)* MAX_LINE);
	int read_line = 0;
	struct candidate* cans = malloc(sizeof(struct candidate)*MAX_NODES);
	int num_candidates = 0;	
	while((numread = readline(votesfile, readbuffer, MAX_LINE)) != 0){
		read_line = 1;
		char **noCommas = malloc(sizeof(char*)*MAX_LINE);
		int noCom = makeargv((trimwhitespace(readbuffer)), ",", &noCommas);
		for(int i = 0; i < noCom; i++){
			//char **noColons = malloc(sizeof(char*)*strlen(noCommas[i]));
			char **noColons = malloc(sizeof(char*)*3);										
			int temp = makeargv(trimwhitespace(noCommas[i]), ":", &noColons);
			int numVotes = atoi(noColons[1]);
			candidates* cnode = findnode(cans, noColons[0]);
			if(cnode == NULL){
				strcpy(cans[num_candidates].name, noColons[0]);		
				cans[num_candidates].count = numVotes;								
				num_candidates++;
			} 		
			else{
				cnode->count = cnode->count + numVotes;
			}
		}
	}
	if(read_line == 0){
		fprintf(stderr, "failed to read file %s\n", path);
	}
	//close(votesfile);
	if(num_candidates <= 0) fprintf(stderr, "nothing to write, no candidates in read file\n");
	if(num_candidates >= 1){
		char *winnerLine = malloc(sizeof(char)* MAX_LINE);
		//int writefile = open(votesfile, O_RDWR);
		//int max = findMax(cans, num_candidates);
		char* winner = findName(cans, num_candidates);
		strcat(winnerLine, "Winner");
		strcat(winnerLine, ":");
		strcat(winnerLine, winner);
		strcat(winnerLine, "\n");
		int numWrite = write(votesfile, trimwhitespace(winnerLine), strlen(trimwhitespace(winnerLine)));
		if(numWrite == -1){
			fprintf(stderr, "fail to write the file %s\n", path);
			exit(EXIT_FAILURE);
		}  
		close(votesfile);
		printf("%s\n", path);		
	}

}

int main(int argc, char **argv){	//main driver

	if (argc != 2){		//incorrect number of arguments
		printf("Usage: %s Program\n", argv[0]);
		return -1;
	}

	int won = who_won(argv[1]);
	if (!won){
		perror("who won function failed to determine winner\n");
		exit(EXIT_FAILURE);
	}
	
	return 0;
	

}

