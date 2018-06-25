#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void sigchld_handler(int signo){}

int main(void){
  // create a signal handler for SIGCHLD
  signal(SIGCHLD, sigchld_handler);
  // fork
  switch(fork()){
    case -1:
      perror("main: fork");
      exit(1);
      break;
    case 0:
      // print pid if child
      printf("%d\n", getpid());
      exit(0);
    default:
      // wait for signal if parent
      pause();
      wait(NULL);
      break;
  }
  return EXIT_SUCCESS;
}

