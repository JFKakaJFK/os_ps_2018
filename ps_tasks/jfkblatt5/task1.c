#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define SHM_SIZE 16
#define FIFO "RESULT_FIFO"
#define SHM_KEY 666
#define CHILDREN 100
#define MAX 100

int main(void) {
  int shm_id, *counter;
  key_t key = SHM_KEY;

  fprintf(stdout, "%d: accessing shared memory segment... ", getpid());

  if( (shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0 ){
    perror("shmget");
    _exit(EXIT_FAILURE);
  }

  if( (counter = shmat(shm_id, NULL, 0)) < 0 ){
    perror("shmat");
    _exit(EXIT_FAILURE);
  }

  fprintf(stdout, "done\n%d: spawing %d child processes... ", getpid(), CHILDREN);

  pid_t cpid;
  for(int i = 0; i < CHILDREN; i++){
    if( (cpid = fork()) < 0){
      perror("fork");
      _exit(EXIT_FAILURE);
    } else if( cpid == 0 ){
      for(int j = 0; j < MAX; j++){
        ++*counter;
      }
      _exit(EXIT_SUCCESS);
    }
  }
  
  fprintf(stdout, "done\n%d: waiting for children... ", getpid());

  while( wait(NULL) > 0);

  fprintf(stdout, "done\n%d: printing result: %d\n", getpid(), *counter);

  fprintf(stdout, "%d: writing to pipe... ", getpid());
  FILE* fifo_fp;

  if( (fifo_fp = fopen(FIFO, "w")) == NULL ){
    perror("fopen");
    _exit(EXIT_FAILURE);
  }

  fprintf(fifo_fp, "%d", *counter);

  if( fclose(fifo_fp) < 0 ){
    perror("fclose");
    return EXIT_FAILURE;
  }

  if( shmdt(counter) < 0){
    perror("shmdt");
    return EXIT_FAILURE;
  }

  if( shmctl(shm_id, IPC_RMID, 0) < 0){
    perror("shmctl");
    return EXIT_FAILURE;
  } 

  fprintf(stdout, "done\n");

  return EXIT_SUCCESS;
}
