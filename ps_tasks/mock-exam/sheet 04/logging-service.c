
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>
#include <limits.h>


int main(void)
{
	
	int data = open("databasefifo", O_RDONLY);
	int middle = open("middlewarefifo", O_RDONLY);
	int web = open("webfifo", O_RDONLY);
	
	while (1) {
	
		fd_set fifos;
		int nfds;
		FD_ZERO(&fifos);
		FD_SET(data, &fifos);
		FD_SET(middle, &fifos);
		FD_SET(web, &fifos);
		
		nfds = data > middle ? data : middle;
		nfds = web > nfds ? web : nfds;
		
		select(nfds + 1, &fifos, NULL, NULL, NULL);
		
		
		if (FD_ISSET(data, &fifos)) {
			FILE *data = fopen("databasefifo", "r");
			char c;
			while ((c = getc(data)) != EOF) {
				putchar(c);
			}
			fclose(data);
		}
		if (FD_ISSET(middle, &fifos)) {
			FILE *middle = fopen("middlewarefifo", "r");
			char c;
			while ((c = getc(middle)) != EOF) {
				putchar(c);
			}
			fclose(middle);
		}
		if (FD_ISSET(web, &fifos)) {
			FILE *web = fopen("webfifo", "r");
			char c;
			while ((c = getc(web)) != EOF) {
				putchar(c);
			}
			fclose(web);
		}
	}
	return EXIT_SUCCESS;
}

