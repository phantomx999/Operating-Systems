#define _BSD_SOURCE
#define NUM_ARGS 3

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#define B_SIZE 1024


void shallowCopy(char* old, char* new) {


	// TODO: Create links to all files in old in new.
	//link()
	struct dirent* dire;	
	DIR* dir;
	dir = opendir(old);
	while( (dire=readdir(dir)) != NULL){
		if(DT_DIR != dire -> d_type || strcmp(dire -> d_name, ".") != 0 || strcmp(dire -> d_name, "..") != 0){
			char* old_buf = malloc(B_SIZE*sizeof(char));
			char* new_buf = malloc(B_SIZE*sizeof(char));
			//char* old_temp_str = malloc(B_SIZE*sizeof(char));
			//char* new_temp_str = malloc(B_SIZE*sizeof(char));
			char old_temp_str[B_SIZE];
			char new_temp_str[B_SIZE];
			
			strcpy(old_temp_str, old);
			strcat(old_temp_str, "/");
			strcat(old_temp_str, dire -> d_name);
			strcpy(new_temp_str, new);
			strcat(new_temp_str, "/");
			strcat(new_temp_str, dire -> d_name);
			
			snprintf(old_buf, B_SIZE, "%s\n", old_temp_str);
			snprintf(new_buf, B_SIZE, "%s\n", new_temp_str);

 			link(old_buf, new_buf);
			free(old_buf);
			free(new_buf);
		}
		
	}
	closedir(dir);
	 
}

void deepCopy(char* old, char* new) {

	// TODO: Copy the contents of all files in old to new.
	//read/write()
	struct dirent* dire;	
	DIR* dir;
	dir = opendir(old);

	while( (dire = readdir(dir)) != NULL){
		if(DT_DIR != dire -> d_type || strcmp(dire -> d_name, ".") != 0 || strcmp(dire -> d_name, "..") != 0){
			int read_call = 0;
			int write_call = 0;			
			char* old_buf = malloc(B_SIZE*sizeof(char));
			char* new_buf = malloc(B_SIZE*sizeof(char));
			char* buffer = malloc(10000*sizeof(char));
			char old_temp_str[B_SIZE];
			char new_temp_str[B_SIZE];
			strcpy(old_temp_str, old);
			strcat(old_temp_str, "/");
			strcat(old_temp_str, dire -> d_name);
			strcpy(new_temp_str, new);
			strcat(new_temp_str, "/");
			strcat(new_temp_str, dire -> d_name);
			
			snprintf(old_buf, B_SIZE, "%s\n", old_temp_str);
			snprintf(new_buf, B_SIZE, "%s\n", new_temp_str);
			
			int source = open(old_buf, O_RDWR);
			int dest = open(new_buf, O_RDWR|O_CREAT);
		
			do{
						fprintf(stderr, "made it here\n");		
				read_call = read(source, buffer, 10000);
				if(read_call == -1){
					perror("read file fail");
					exit(1);
				}
				write_call = write(dest, buffer, read_call);
				if(write_call == -1){
					perror("write file fail");
					exit(1);
				} 				
			}while (read_call != 0);
			
		}
	}
	closedir(dir);
}

int main(int argc, char** argv) {

	if (argc < NUM_ARGS + 1) {

		printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
		exit(1);
	}

	// TODO: Time how long it takes to execute each copy and print the results.
	
    	clock_t start, finish;
	double shallow_time_used, deep_time_used;
	start = clock();
	shallowCopy(argv[1], argv[2]);
	finish = clock();
	shallow_time_used = ((double) finish - start)/CLOCKS_PER_SEC;

	start = clock();
	deepCopy(argv[1], argv[3]);
	finish = clock();
	deep_time_used = ((double) finish - start)/CLOCKS_PER_SEC;

	printf("Shallow time used is %f and deep time used is %f\n\n", shallow_time_used, deep_time_used);
}
