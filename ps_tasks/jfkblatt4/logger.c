#define _POSIX_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/select.h>

// later use argv[argc - 2] ?
#define PIPE_BUF 32

// later use argv[argc - 1] ?
#define SERVERS 3

// argv 1 to servers need to be pipe names
int main(int argc, char *argv[]){
  fprintf(stdout, "logging service started...\n");

  char *fifos[SERVERS];
  FILE *fifo_fps[SERVERS];
  int fifo_fds[SERVERS];

  // use the pipe names to get the FILE* and use those to
  // get the file descriptors and save all in an array
  for(int i = 1; i <= SERVERS; i++){
    fifos[i] = argv[i];
    if( (fifo_fps[i] = fopen(fifos[i], "r")) == NULL){
      perror("fopen");
      _exit(EXIT_FAILURE);
    }
    if( (fifo_fds[i] = fileno(fifo_fps[i])) == -1){
      perror("fileno");
      _exit(EXIT_FAILURE);
    }
  }

  char buffer[PIPE_BUF];

  fd_set fds;
  int maxfd = 0;

  for(;;){
    FD_ZERO(&fds);
    for(int i = 0; i < SERVERS; i++){
      FD_SET(fifo_fds[i], &fds);
      maxfd = fifo_fds[i] > maxfd ? fifo_fds[i] : maxfd;
    }

    select(maxfd + 1, &fds, NULL, NULL, NULL);

    for(int i = 0; i < SERVERS; i++){
      if(FD_ISSET(fifo_fds[i], &fds)){
        if( read(fifo_fds[i], buffer, sizeof(buffer)) > 0) {
          fprintf(stdout, "%s\n", buffer);
        }
      }
    }
  }
}

