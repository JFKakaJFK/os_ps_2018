#define _GNU_SOURCE
#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <semaphore.h> // needs -pthreads compile flag
#include <fcntl.h>

#define SHM_SIZE 16
#define FIFO "RESULT_FIFO"
#define SHM_KEY 666
#define CHILDREN 100
#define MAX 100

int main(void) {

  fprintf(stdout, "%d: accessing shared memory segment... ", getpid());

  int shm_id, *counter;
  key_t key = SHM_KEY;

  if( (shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0 ){
    perror("shmget");
    _exit(EXIT_FAILURE);
  }

  if( (counter = shmat(shm_id, NULL, 0)) < 0 ){
    perror("shmat");
    _exit(EXIT_FAILURE);
  }


  fprintf(stdout, "done\n%d: creating semaphore... ", getpid());

  sem_t *sem;

  if( (sem = shmat(shm_id, NULL, 0)) < 0){
    perror("shmat");
    _exit(EXIT_FAILURE);
  }

  if( sem_init(sem, 1, 1) < 0){
    perror("sem_init");
    _exit(EXIT_FAILURE);
  }

  fprintf(stdout, "done\n%d: spawing %d child processes... ", getpid(), CHILDREN);

  pid_t cpid;
  for(int i = 0; i < CHILDREN; i++){
    if( (cpid = fork()) < 0){
      sem_destroy(sem);
      perror("fork");
      _exit(EXIT_FAILURE);
    } else if( cpid == 0 ){
      for(int j = 0; j < MAX; j++){
        sem_wait(sem);
        fprintf(stdout, "\t%d: entering critical region... ", getpid());
        ++*counter;
        fprintf(stdout, "leaving now\n");
        sem_post(sem);
      }
      sem_destroy(sem);
      _exit(EXIT_SUCCESS);
    }
  }
  
  fprintf(stdout, "done\n%d: waiting for children... ", getpid());

  while( wait(NULL) > 0);

  if( sem_destroy(sem) < 0){
    perror("sem_destroy");
    return EXIT_FAILURE;
  }

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

  fprintf(stdout, "done\n");

  if( shmdt(counter) < 0){
    perror("shmdt");
    return EXIT_FAILURE;
  }

  if( shmdt(sem) < 0){
    perror("shmdt");
    return EXIT_FAILURE;
  }

  if( shmctl(shm_id, IPC_RMID, 0) < 0){
    perror("shmctl");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
