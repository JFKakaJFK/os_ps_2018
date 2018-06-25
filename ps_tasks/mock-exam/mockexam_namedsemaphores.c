#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>  
#include <semaphore.h>


#define err(msg) perror(msg); _exit(EXIT_FAILURE);

#define SEMNAME "/semaphore"

int main(void) {
    int shmid;
    key_t key = 5678; // id of shm segment is "5678"
    int *shm;
    sem_t *sem;


    // creating the segment
    if ((shmid = shmget(key, sizeof(int)+sizeof(*sem), IPC_CREAT | 0666)) < 0) { err("shmget"); }

    // attach the segment to our data space
    if ((shm = shmat(shmid, NULL, 0)) == (void *) -1) { err("shmat"); }
    // attach the semaphore
    if( (sem = shmat(shmid, NULL, 0)) < 0){ err("shmat"); }
    
    // inits and locks the semaphore
    sem = sem_open(SEMNAME, O_CREAT, 0666, 1);

    // initialize the single integer with 0 (sub-task 1)
    *shm = 0;



    // code for the spawning of the child process (sub-task 2)
    pid_t child;
    if( (child = fork()) == -1){
        perror("fork");
        return EXIT_FAILURE;
    } else if (child == 0) {
        printf("I bims, the child\n");
        if ((shm = shmat(shmid, NULL, 0)) == (void *) -1) { err("shmat"); }

        //incrementing the value in the shared memory segment 1001 times
        for (int i = 0; i < 1001; i++) {
            if( sem_wait(sem) < 0){ err("sem_wait"); }
            ++*shm;
            if( sem_post(sem) < 0){ err("sem_post"); }
        }

        // detatch the shm from the data space
        if( shmdt(shm) < 0){ err("shmdt"); }

        _exit(EXIT_SUCCESS);     // _exit() is atomic
    }


    fprintf(stdout, "I bims, the parent\n");

    for (int i = 0; i < 1000; i++) {
            if( sem_wait(sem) < 0){ err("sem_wait"); }
            --*shm;
            if( sem_post(sem) < 0){ err("sem_post"); }
    }

    waitpid(child, NULL, 0);      // waits only for child

    // deallocate the shm segment when no one is attached anymore
    // could naturally be done later
    if (shmctl(shmid, IPC_RMID, 0) < 0){ err("shmctl"); }

    printf("Result: %d\n",*shm);

    sem_unlink(SEMNAME);

    // detatch the shm from the data space
    if( shmdt(shm) < 0){ err("shmdt"); }



    return EXIT_SUCCESS;
}
