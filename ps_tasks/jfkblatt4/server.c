#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>

// emulates a very basic server:
int main(int argc, char *argv[]){
  fprintf(stdout, "%s%s started...\n", argv[0], argv[1]);
  FILE *fifo;

  // if fifo does not exist, make fifo

  // open pipe for reading
  if( (fifo = fopen(argv[2], "w")) == NULL){
    perror("fopen");
    _exit(EXIT_FAILURE);
  }

  srand(time(NULL));
  int sleeptime;

  while(1){
    sleeptime = rand() % 6 + 2;
    sleep(sleeptime);

    // send message to fifo fd
    fprintf(fifo, "[%s%s]", argv[0], argv[1]);
    fprintf(stdout, "sent message\n");
  }

  if( fclose(fifo) == EOF){
    perror("fclose");
    _exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}

