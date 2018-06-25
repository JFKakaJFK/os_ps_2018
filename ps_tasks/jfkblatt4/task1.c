#define _POSIX_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define SERVERS 3

// TODO autogenerate fifo+num up to servers
#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define FIFO3 "fifo3"

int main(void){
  char* fifos[SERVERS] = { FIFO1, FIFO2, FIFO3 };

  // open fifos
  for(int i = 0; i < SERVERS; i++){
    if( mkfifo(fifos[i], 0777) == -1){
      perror("mkfifo");
      return EXIT_FAILURE;
    }
  }

  pid_t cpid[SERVERS + 1];
  for(int i = 0; i < SERVERS; i++){
    if( (cpid[i] = fork()) == -1 ){
      perror("fork");
      _exit(EXIT_FAILURE);
    } else if (cpid == 0) {
      execl("server", "server", i, fifos[i], NULL);

      perror("execl");
      _exit(EXIT_FAILURE);
    }
  }

  if( (cpid[SERVERS] = fork()) == -1 ){
    perror("fork");
    _exit(EXIT_FAILURE);
  } else if( cpid == 0 ){
    char* argv[SERVERS + 2];
    argv[0] = "logger";
    for(int i = 1; i < SERVERS; i++){
      argv[i] = fifos[i];
    }
    argv[SERVERS + 1] = NULL;
    execv("logger", argv);

    perror("execv");
    _exit(EXIT_FAILURE);
  } 

  sleep(20);
  for(int i = 0; i < (SERVERS + 1); i++){
    kill(cpid[i], SIGTERM);
  }
    
  // wait for the children
  while(wait(NULL) > 0);

  // unlink the fifos
  for(int i = 0; i < SERVERS; i++){
    if( unlink(fifos[i]) == -1){
      perror("unlink");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

