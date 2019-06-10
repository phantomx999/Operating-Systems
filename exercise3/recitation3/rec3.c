#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
 

	if (argc < 2) {

		printf("Incorrect number of args: given %d, expected 1.\n", argc - 1);
		exit(1);
	}


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

	// TODO: Fill in your code here.
	pid_t pid1 = fork();
	if(pid1 == -1){
		perror("Error, could not fork\n");
		exit(1);
	}	
	else if(pid1 > 0){
		wait(NULL);
		printf("Parent: wait pid1\n");
	}
	else if(pid1 == 0){
		execl("rtime.o", "./rtime.o", time_str, NULL);
		printf("child number 1 failed (execl)\n");			
	}


	pid_t pid2 = fork();
	if(pid2 == -1){
		perror("Error, could not fork\n");
		exit(1);
	}
	else if(pid2 > 0){
		wait(NULL);
		printf("Parent: wait pid2\n");
	}
	else if(pid2 == 0){	
		char* ar[] = {"./rtime.o", time_str, NULL};
		execv("rtime.o", ar);
		printf("child number 2 failed (execv)\n");	
	}
	

}
