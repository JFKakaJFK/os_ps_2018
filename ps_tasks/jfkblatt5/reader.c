#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define BUF_SIZE 16
#define SHM_SIZE 16
#define FIFO "RESULT_FIFO"
#define SHM_KEY 666

int main(void) {
  // needs to be int* since shmat() won't accept &int
  int shm_id, *counter;
  key_t key = SHM_KEY;

  fprintf(stdout, "%d: creating shared memory segment... ", getpid());

  if( (shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0 ){
    perror("shmget");
    return EXIT_FAILURE;
  }

  if( (counter = shmat(shm_id, NULL, 0)) < 0 ){
    perror("shmat");
    return EXIT_FAILURE;
  }

  *counter = 0;

  fprintf(stdout, "done\n%d: creating fifo... ", getpid());

  FILE* fifo_fp;

  unlink(FIFO);

  if( mkfifo(FIFO, 0777) == -1){
    perror("mkfifo");
    return EXIT_FAILURE;
  }

  fprintf(stdout, "done\n%d: waiting for fifo...\n", getpid());

  if( (fifo_fp = fopen(FIFO, "r")) == NULL ){
    perror("fopen");
    return EXIT_FAILURE;
  }

  // read from fifo to buffer, convert buffer to long
  char buf[BUF_SIZE], *end;
  if( read(fileno(fifo_fp), buf, sizeof(buf)) > 0){
    fprintf(stdout, "%d: received something: %ld\n", getpid(), strtol(buf, &end, 10));
  }
  
  // close fifo
  if( fclose(fifo_fp) < 0){
    perror("fclose");
    return EXIT_FAILURE;
  }

  // unlink fifo
  if( unlink(FIFO) == -1){
    perror("unlink");
    return EXIT_FAILURE;
  }

  // detatch counter from shared memory
  if( shmdt(counter) < 0){
    perror("shmdt");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

