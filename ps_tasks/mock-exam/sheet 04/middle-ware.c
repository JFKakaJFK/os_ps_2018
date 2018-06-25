#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main(void)
{
	FILE *my_fifo;
	int fifo = mkfifo("middlewarefifo", 0777);
	if (fifo == -1) {
		fprintf(stderr,"Could not create FIFO!\n");
		exit(0);
	}
	
	srand(time(NULL));
	
	int message_number = 0;
	
	while (1) {
		message_number++;
		if ((my_fifo =fopen("middlewarefifo", "w")) == NULL) {
			fprintf(stderr,"Could not open FIFO!\n");
		}
		
		printf("Sending message...\n");
		
		fprintf(my_fifo, "Message %d from middle_ware server\n", message_number);
		
		if(fclose(my_fifo) == EOF) {
			fprintf(stderr,"Could not open FIFO!\n");
		}
		
		int quantity = rand() % 6 +2;
		sleep(quantity);
	}
	return EXIT_SUCCESS;
}
