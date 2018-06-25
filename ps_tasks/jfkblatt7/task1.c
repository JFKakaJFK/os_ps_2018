#define _POSIX_SOURCE
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <ctype.h>
#include <pthread.h>
#include "myqueue.h"

#define gettid() syscall(SYS_gettid)

#define err_t(msg) \
  perror(msg); pthread_exit(NULL)

#define err(msg) \
  perror(msg); _exit(EXIT_FAILURE)

#define THREADS 5
#define NUM_ENTRYS 10000

pthread_spinlock_t spinlock;

void* consumer(void *arg){
  int sum = 0, read;
  while(1){
    pthread_spin_lock(&spinlock);
    // read
    if(!empty()){
      read = front();
      pop();
      // check wheter to terminate
      if(read){
        sum += read;
      } else {
        pthread_spin_unlock(&spinlock);
        break;
      }
    }
    pthread_spin_unlock(&spinlock);
  }
  fprintf(stdout, "%ld: %d\n", gettid(), sum);
  pthread_exit(NULL);
}

int main (void){
  pthread_t tid[THREADS];

  if( pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE) != 0){
    err("pthread_spin_init");
  }

  fprintf(stdout, "creating queue... ");

  create();

  // create THREADS threads
  fprintf(stdout, "done\ncreating %d threads... ", THREADS);
  for(int i = 0; i < THREADS; i++){
    if( pthread_create(&(tid[i]), NULL, &consumer, NULL) != 0){
      err("pthread_create");
    }
  }

  fprintf(stdout, "done\nwriting to queue...\n");

  for(int i = 0; i < NUM_ENTRYS; i++){
    pthread_spin_lock(&spinlock);
    // write
    push(1);
    pthread_spin_unlock(&spinlock);
  }
  for(int i = 0; i < THREADS; i++){
    pthread_spin_lock(&spinlock);
    // write
    push(0);
    pthread_spin_unlock(&spinlock);
  }

  fprintf(stdout, "done\nwaiting for threads to finish...\n");
  // wait for threads
  for(int i = 0; i < THREADS; i++){
    pthread_join(tid[i], NULL);
  }
  fprintf(stdout, "done\ndestroying spinlock...");
  pthread_spin_destroy(&spinlock);
  fprintf(stdout, "done\n");
  return EXIT_SUCCESS;
}
