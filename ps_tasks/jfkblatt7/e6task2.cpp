#define _POSIX_SOURCE
// #define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <ctype.h>
#include <pthread.h>

#include <queue>
#include <iostream>

#define gettid() syscall(SYS_gettid)

#define err_t(msg) \
  perror(msg); pthread_exit(NULL)

#define err(msg) \
  perror(msg); _exit(EXIT_FAILURE)

#define THREADS 5
#define NUM_ENTRYS 10000

pthread_mutex_t lock;
std::queue<unsigned> queue;

void* consumer(void *arg){
  int sum = 0, read;
  while(1){
    // mutex
    pthread_mutex_lock(&lock);
    // read
    if(!queue.empty()){
      read = queue.front();
      queue.pop();
      // check wheter to terminate
      if(read){
        sum += read;
      } else {
        pthread_mutex_unlock(&lock);
        break;
      }
    }
    // mutex
    pthread_mutex_unlock(&lock);
  }
  fprintf(stdout, "%ld: %d\n", gettid(), sum);
  pthread_exit(NULL);
}

int main (void){
  pthread_t tid[THREADS];

  fprintf(stdout, "creating queue... ");

  // create THREADS threads
  fprintf(stdout, "done\ncreating %d threads... ", THREADS);
  for(int i = 0; i < THREADS; i++){
    // TODO queue and mutex as arg -> struct
    if( pthread_create(&(tid[i]), NULL, &consumer, NULL) != 0){
      err("pthread_create");
    }
  }

  fprintf(stdout, "done\nwriting to queue...\n");

  for(int i = 0; i < NUM_ENTRYS; i++){
    // mutex
    pthread_mutex_lock(&lock);
    // write
    queue.push(1);
    // mutex
    pthread_mutex_unlock(&lock);
  }
  for(int i = 0; i < THREADS; i++){
    // mutex
    pthread_mutex_lock(&lock);
    // write
    queue.push(0);
    // mutex
    pthread_mutex_unlock(&lock);
  }

  fprintf(stdout, "done\nwaiting for threads to finish...\n");
  // wait for threads
  for(int i = 0; i < THREADS; i++){
    pthread_join(tid[i], NULL);
  }
  fprintf(stdout, "done\ndestroying mutex...");
  // destroy mutex
  pthread_mutex_destroy(&lock);
  fprintf(stdout, "done\n");
  return EXIT_SUCCESS;
}
