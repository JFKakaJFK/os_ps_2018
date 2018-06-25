#define _POSIX_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h> // for )_ macros
#include <signal.h>

#define err(msg) perror(msg); _exit(EXIT_FAILURE)

#define PIPES 3
#define BUF_SIZE 64
#define NAMEBUF 32

volatile sig_atomic_t running = 1;

void sigint_h(int signum){
  if(signum == SIGINT){
    running = 0;
  }
}

int main(int argc, char *argv[]){

  FILE *fifo_fp[PIPES];
  char buf[BUF_SIZE];
  char fn[NAMEBUF] = {0};

  for(int i = 0; i < PIPES; i++){
    sprintf(fn, "/tmp/FIFO_%d", i);
    puts(fn);
    if( (fifo_fp[i] = fopen(fn, "r")) == NULL){ err("fopen"); }
  }

  fd_set fds;
  int maxfd;

  while(running){
    FD_ZERO(&fds);
    for(int i = 0; i < PIPES; i++){
      FD_SET(fileno(fifo_fp[i]), &fds);
    }

    maxfd = fileno(fifo_fp[0]);
    for(int i = 1; i < PIPES; i++){
      maxfd = maxfd > fileno(fifo_fp[i]) ? maxfd : fileno(fifo_fp[i]); 
    }

    select(maxfd + 1, &fds, NULL, NULL, NULL);

    for(int i = 0; i < PIPES; i++){
      if(FD_ISSET(fileno(fifo_fp[i]), &fds)){
        if(fscanf(fifo_fp[i], buf, sizeof(buf)) > 0){
          fprintf(stdout, "%s\n", buf);
          buf[0] = 0;
        }
      }
    }
  }

  // closing the pipe (should never get here)
  for(int i = 0; i < PIPES; i++){
    if( fclose(fifo_fp[i]) == EOF){ err("fclose"); }
    sprintf(fn, "/tmp/FIFO_%d", i);
    if( unlink(fn) == -1){ err("unlink"); }
  }

  return EXIT_SUCCESS;
}
