#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char**argv) {
	
	int fd[2];
	int x;
	
	//arguments for ls and grep
	char *ls[] = {"ls", NULL};
	char *grep[] = {"grep", argv[1], NULL};
	
	
	if (pipe(fd) == -1) {
		printf("pipe failed!");
	}
	
	x = fork();
	
	switch(x) {
	case -1:
		printf("fork failed!\n");
		exit(1);
	case 0:
		dup2(fd[1],1);
		close(fd[0]);
		if (execv("/bin/ls",ls) == -1) {
			printf("execv failed!\n");
		}
		break;
	default:
		close(fd[1]);
		while (wait(0) != x) {}
		dup2(fd[0],0);
		if (execv("/bin/grep", grep) == -1) {
			printf("execv failed\n");
		}
	}
	return EXIT_SUCCESS;
}
