#define _POSIX_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h> // for _ macros

#define err(msg) perror(msg); _exit(EXIT_FAILURE)
#define NAMEBUF 32
#define MESSAGE "i bims"
#define PIPEBUF 128

volatile sig_atomic_t running = 1;

void sigint_h(int signum){
  if (signum == SIGINT){
    running = 0;
  }
}

int main(int argc, char *argv[]){
  if(argc != 2){ 
    fprintf(stdout, "Usage: ./%s <FIFONUM>\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  FILE *fifo_fp;
  char fn[NAMEBUF] = {0},
       buf[PIPEBUF] = {0};
  sprintf(fn, "/tmp/FIFO_%s", argv[1]);

  if( mkfifo(fn, 0666) == -1){
    fprintf(stdout, "%dFifo already exists\n", getpid());
  }
  if( (fifo_fp = fopen(fn, "w")) == NULL){ err("fopen"); }

  while(running){
    sleep(rand() % 6 + 2);
    //fprintf(fifo_fp, "%d: i bims.\n", getpid());
    sprintf(buf, "%d:%s\n", getpid(), MESSAGE);
    write(fileno(fifo_fp), buf, PIPEBUF+1);
    fprintf(stdout, "%d: wrote to fifo\n", getpid());
  }

  // closing the pipe
  if( fclose(fifo_fp) == EOF){ err("fclose"); }
  if( unlink(fn) == -1){ err("unlink"); }
             
  return EXIT_SUCCESS;
}
