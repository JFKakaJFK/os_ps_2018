#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROCESSES 9

int main(void){
  pid_t child_pid;

  // create 9 processes and store their pids
  for(int i = 0; i < PROCESSES; i++){
    // create child, exit if child or on error
    if((child_pid = fork()) == 0){
      exit(0);
    } else if (child_pid == -1){
      exit(1);
    }
  }

  // wait while the wait call returns no error, don't check exit status of
  // children
  while(wait(NULL) > 0); 

  return EXIT_SUCCESS;
}
