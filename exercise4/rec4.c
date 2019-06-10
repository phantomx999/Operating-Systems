#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {

	if (argc < 2) {

		printf("Wrong number of args, expected 1, given %d", argc - 1);
		exit(1);
	}

	int fd[2];	
	pipe(fd);

	if (pipe(fd) < 0) {
	    perror("pipe error");
	    exit(EXIT_FAILURE);
	}

	int child_file_descriptor;
	char * child_file = "output.txt";

	int time = 2 *atoi(argv[1]);
	int count_digits = 0;
	int temp = time;
	if(temp == 0)
		count_digits = 1; 	
	else{
		while(temp > 0){
			temp = temp/10;
			count_digits++;
		}
	}
	char time_str[count_digits];
	sprintf(time_str, "%d", time); 


	// TODO: Your code goes here.
	pid_t pid1 = fork();

	if(pid1 == -1){
		perror("Error, could not fork\n");
		exit(EXIT_FAILURE);
	}	
	else if(pid1 > 0){
		wait(NULL);
		printf("Parent: wait pid1\n");
		close(fd[1]);
		char buffer[1000];
		read(fd[0], child_file, sizeof(child_file));
		//read(fd[0], &fd, sizeof(fd));
		chmod(child_file, 0700);
		close(fd[0]);
		int parent_received_file = open(child_file, O_RDONLY);
		int time_stamp = 0;
		int time_stamp_array[time];
		int line = 0;
		for(int i = 0; i < time; i++){
			line = read(parent_received_file, buffer+11*i, 11);
			time_stamp = atoi((i*11) + buffer);	
			time_stamp_array[i] = time_stamp;
			if(i > 0 && i < time){
				if (time_stamp_array[i] <= time_stamp_array[i-1]){
					printf("incorrect output, time stamps are incorrect\n");
					exit(1);
				}
			}
		} 
		printf("correct output\n");
	}
	else if(pid1 == 0){
		close(fd[0]);
		child_file_descriptor = open(child_file, O_RDWR|O_CREAT);
		printf("reach child process right before dup2\n");
		printf("timestring: %s\n", time_str);
		dup2(child_file_descriptor, 1);
	        write(fd[1], child_file, sizeof(child_file));
		close(fd[1]);
		execl("rec3.o", "./rec3.o", time_str, NULL);
		printf("child number 1 failed (execl)\n");			
	
	//read(fd, buffer, int);
	//file 	
	}	
	return 0;
	
	
       
	
	
}
