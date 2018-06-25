#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // for )_ macros
#include <sys/select.h>
#include <time.h>
#include "string.h"

#define err(msg) perror(msg); _exit(EXIT_FAILURE)

#define FIFO1 "/tmp/FIFO_NAME3"



int main(void)
{


  FILE *myfifo;
  if (mkfifo(FIFO1, 0777) < 0) {
    fprintf(stdout,"Could not create FIFO!\n");
    fprintf(stdout,"FIFO exists!\n");
    //exit(0);
  }

  srand(time(NULL));
  
  int message_number = 0;

  myfifo = fopen(FIFO1, "w");
      //fprintf(stdout,"Could not open FIFO!\n");
  printf("Hallo\n");
  for(;;) {
    /*
    if ((my_fifo = fopen(FIFO1, "w")) == NULL) {
      fprintf(stderr,"Could not open FIFO!\n");
    }
    */
    printf("Sending message...\n");
    
    fprintf(myfifo, "Message %d from database\n", message_number);
    
    //if(fclose(my_fifo) == EOF) {
    //  fprintf(stderr,"Could not open FIFO!\n");
    //}
    printf("Hallo\n");
    int quantity = rand() % 6 +2;
    sleep(quantity);
    message_number++;
  }
  if(fclose(myfifo) == EOF) {
    fprintf(stderr,"Could not open FIFO!\n");
  }
  return EXIT_SUCCESS;
}
