#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // for )_ macros
#include <sys/select.h>

#define err(msg) perror(msg); _exit(EXIT_FAILURE)

#define FIFO1 "/tmp/FIFO_NAME1"
#define FIFO2 "/tmp/FIFO_NAME2"
#define FIFO3 "/tmp/FIFO_NAME3"


int main(void) {

int fifo_fp1;
int fifo_fp2;
int fifo_fp3;

if( (fifo_fp1 = open(FIFO1, O_RDONLY)) < 0){ err("fopen"); }

if( (fifo_fp2 = open(FIFO2, O_RDONLY)) < 0){ err("fopen"); }

if( (fifo_fp3 = open(FIFO3, O_RDONLY)) < 0){ err("fopen"); }

for (;;)
{
    fd_set fifos;
    int nfds;
    FD_ZERO(&fifos);
    FD_SET(fifo_fp1, &fifos);
    FD_SET(fifo_fp2, &fifos);
    FD_SET(fifo_fp3, &fifos);
    
    nfds = fifo_fp1 > fifo_fp2 ? fifo_fp1 : fifo_fp2;
    nfds = fifo_fp3 > nfds ? fifo_fp3 : nfds;
    
    select(nfds + 1, &fifos, NULL, NULL, NULL);
    
    
    if (FD_ISSET(fifo_fp1, &fifos)) {
      FILE *fifo_fp1 = fopen("FIFO1", "r");
      char c;
      while ((c = getc(fifo_fp1)) != EOF) {
        putchar(c);
      }
      fclose(fifo_fp1);
    }
    if (FD_ISSET(fifo_fp2, &fifos)) {
      FILE *fifo_fp2 = fopen(FIFO2, "r");
      char c;
      while ((c = getc(fifo_fp2)) != EOF) {
        putchar(c);
      }
      fclose(fifo_fp2);
    }
    if (FD_ISSET(fifo_fp3, &fifos)) {
      FILE *fifo_fp3 = fopen(FIFO3, "r");
      char c;
      while ((c = getc(fifo_fp3)) != EOF) {
        putchar(c);
      }
      fclose(fifo_fp3);
    }
}
  return 0;
}
