#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#define PROCESSES 16

int main(void){
  pid_t pid;
  // create 16 child processes
  for(int i = 0; i < PROCESSES; i++){
    //create fork, if child print pid
    if((pid = fork()) == 0){
      // print child pid
      printf("pid: %d\n", getpid());
      exit(0);
    } else if(pid == -1){
      exit(1);
    }
  }
  /*
   * In my analysis, the order of the pid messages is generally ascending
   * altough not always perfectly ordered.
   * The parents message is normally in the middle of the other messages.
   *
   * The order cannot be predicted perfectly, other than generally the sooner
   * a process starts, the sooner it will finish in most cases. It depends on
   * many factors like the scheduling algorithm, cpu usage,...
   */
  printf("%d child processes have been created\n", PROCESSES);

  // wait for all children
  while(wait(NULL) > 0);

  return EXIT_SUCCESS;
}
