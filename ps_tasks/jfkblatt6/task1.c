#define _POSIX_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h> // compile with -pthread
#include <sys/syscall.h>
#include <string.h>

#define gettid() syscall(SYS_gettid)

#define THREADS 10
#define FNAME_SIZE 32

// error handling
#define handle_error(msg) \
  perror(msg); _exit(EXIT_FAILURE)

#define handle_error_t(msg) \
  perror(msg); pthread_exit(NULL)

void cleanup_handler(void *fp){
  if (fclose((FILE *) fp) != 0){ handle_error_t("fclose"); }
}

void* writeToFile(void *n){
  sleep(rand()%4);

  // cannot be canceled
  if( pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL) != 0){
    handle_error_t("pthread_setcancelstate");
  }

  // get filename
  char fname[FNAME_SIZE];
  snprintf(fname, sizeof(fname), "thread%d.txt", *((int *) n));

  // open file
  FILE *fp;
  if( (fp = fopen(fname, "w")) == NULL){
    handle_error_t("fopen");
  }

  // if the thread gets canceled, close the file
  pthread_cleanup_push(cleanup_handler, fp);


  // can be canceled again
  if( pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) != 0){
    handle_error_t("pthread_setcancelstate");
  }

  // write to file
  fprintf(stdout, "%ld\n", gettid());

  // fprintf(fp, "%lu\n", pthread_self());
  fprintf(fp, "%ld\n", gettid());

  // close file
  pthread_cleanup_pop(1);

  // exit thread
  pthread_exit(NULL);
}

int main (void){
  srand(time(NULL));
  int err, tnum[THREADS];
  pthread_t tid[THREADS];

  // create THREADS threads
  fprintf(stdout, "creating %d threads... ", THREADS);
  for(int i = 0; i < THREADS; i++){
    tnum[i] = i;
    if( (err = pthread_create(&(tid[i]), NULL, &writeToFile, (void *) &tnum[i])) != 0){
      handle_error("pthread_create");
    }
  }

  fprintf(stdout, "done\ncanceling threads...\n");

  // for each thread decide if it gets cancelled
  for(int i = 0; i < THREADS; i++){
    if(rand()%2){
      fprintf(stdout, "\tthread %d got canceled\n", i);
      if (pthread_cancel(tid[i]) != 0){
        handle_error("pthread_cancel");
      }
    }
  }

  fprintf(stdout, "done\nwaiting for threads to finish...\n");

  // wait for all threads
  for(int i = 0; i < THREADS; i++){
    pthread_join(tid[i], NULL);
  }
  fprintf(stdout, "done\n");

  return EXIT_SUCCESS;
}

