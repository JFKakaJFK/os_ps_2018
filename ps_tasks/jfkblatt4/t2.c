#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * ls [1] -> pipe -> [0] grep <keyword>
 */

int main(int argc, char *argv[]){
  if(argc != 2){
    fprintf(stderr, "Usage: ./%s <keyword>\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  pid_t cpid;
  int pipefd[2];

  // create the pipe
  if(pipe(pipefd) == -1){ perror("pipe"); return EXIT_FAILURE;}
  // fork the process
  if((cpid = fork()) == -1){ perror("fork"); return EXIT_FAILURE;}

  if(!cpid){
    close(pipefd[0]);   // close read end
    dup2(pipefd[1], 1); // duplicate stdin as write end
    close(pipefd[1]);   // close excess write end

    // call ls
    execlp("ls", "ls", NULL);

    perror("execlp");
    _exit(EXIT_FAILURE);
  } else {
    close(pipefd[1]);   // close read end
    dup2(pipefd[0], 0); // dup2 stdout as read end
    close(pipefd[0]);   // close excess write end

    // call grep
    execlp("grep", "grep", argv[1], NULL);

    perror("execlp");
    _exit(EXIT_FAILURE);
  }
}
