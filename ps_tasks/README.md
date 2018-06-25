# 1. PS Operating Systems

A (more comprehensive than the lecture) summary of the lecture notes about:

- Processes
- IPC (without [message queues](http://man7.org/linux/man-pages/man2/msgrcv.2.html))
- Threads

**NOTE:** None of the examples have been tested, but they should work.

<!-- TOC -->

- [1. PS Operating Systems](#1-ps-operating-systems)
    - [1.1. Processes and IPC](#11-processes-and-ipc)
        - [1.1.1. Creating and Terminating Processes](#111-creating-and-terminating-processes)
            - [1.1.1.1. fork() and other basics](#1111-fork-and-other-basics)
            - [1.1.1.2. Example: fork()](#1112-example-fork)
            - [1.1.1.3. exec()](#1113-exec)
            - [1.1.1.4. Example: exec()](#1114-example-exec)
        - [1.1.2. Signals and Signal Handling](#112-signals-and-signal-handling)
            - [1.1.2.1. Synchronous Signals](#1121-synchronous-signals)
            - [1.1.2.2. Asynchronous Signals](#1122-asynchronous-signals)
            - [1.1.2.3. Sending and receiving Signals](#1123-sending-and-receiving-signals)
            - [1.1.2.4. Example: signal()](#1124-example-signal)
            - [1.1.2.5. The hard (and clean) way](#1125-the-hard-and-clean-way)
            - [1.1.2.6. Example: sigaction()](#1126-example-sigaction)
        - [1.1.3. Named and Unnamed Pipes](#113-named-and-unnamed-pipes)
            - [1.1.3.1. Unnamed Pipes](#1131-unnamed-pipes)
            - [1.1.3.2. Example: pipe()](#1132-example-pipe)
            - [1.1.3.3. Named Pipes(FIFOs)](#1133-named-pipesfifos)
            - [1.1.3.4. Example: mkfifo()](#1134-example-mkfifo)
        - [1.1.4. System V vs. POSIX](#114-system-v-vs-posix)
        - [1.1.5. Semaphores and Shared Memory](#115-semaphores-and-shared-memory)
            - [1.1.5.1. Shared Memory](#1151-shared-memory)
            - [1.1.5.2. System V](#1152-system-v)
            - [1.1.5.3. Example: System V shared memory](#1153-example-system-v-shared-memory)
            - [1.1.5.4. POSIX](#1154-posix)
            - [1.1.5.5. Semaphores](#1155-semaphores)
            - [1.1.5.6. System V](#1156-system-v)
            - [1.1.5.7. Example: System V semaphores](#1157-example-system-v-semaphores)
            - [1.1.5.8. POSIX](#1158-posix)
            - [1.1.5.9. Example: unnamed POSIX semaphore(with shared memory)](#1159-example-unnamed-posix-semaphorewith-shared-memory)
    - [1.2. Threads](#12-threads)
        - [1.2.1. Creating Threads](#121-creating-threads)
            - [1.2.1.1. Example: pthread_create()](#1211-example-pthread_create)
        - [1.2.2. Mutual Exclusion](#122-mutual-exclusion)
            - [1.2.2.1. Mutexes](#1221-mutexes)
            - [1.2.2.2. Example: pthread_mutex()](#1222-example-pthread_mutex)
        - [1.2.3. Spinlocks](#123-spinlocks)
            - [1.2.3.1. Example: pthread_spinlock()](#1231-example-pthread_spinlock)
        - [1.2.4. Condition Variables](#124-condition-variables)
            - [1.2.4.1. Example: pthread_cond_t](#1241-example-pthread_cond_t)
    - [1.3. Usage Patters](#13-usage-patters)
        - [1.3.1. Server - Clients](#131-server---clients)
            - [1.3.1.1. Example: Task 4.1](#1311-example-task-41)
        - [1.3.2. Producer - Consumers](#132-producer---consumers)
            - [1.3.2.1. Example: Task 6](#1321-example-task-6)
    - [1.4. Task Overview](#14-task-overview)
        - [1.4.1. Solutions](#141-solutions)
            - [1.4.1.1. Task 1](#1411-task-1)
            - [1.4.1.2. Task 2](#1412-task-2)
            - [1.4.1.3. Task 3](#1413-task-3)
            - [1.4.1.4. Task 4](#1414-task-4)
            - [1.4.1.5. Task 5](#1415-task-5)
            - [1.4.1.6. Task 6](#1416-task-6)
            - [1.4.1.7. Task 7](#1417-task-7)
            - [1.4.1.8. Task 8](#1418-task-8)
            - [1.4.1.9. Task 9](#1419-task-9)
            - [1.4.1.10. Task 10](#14110-task-10)
    - [1.5. Further Reading](#15-further-reading)

<!-- /TOC -->

## 1.1. Processes and IPC

### 1.1.1. Creating and Terminating Processes

A process is an instance of a program.
In order to execute, a process needs resources(CPU, Memory, Files,...).
Each process has a *Process Control Block* with:

- An unique ID (a operating system only has a limited range of pids -> limit for max number of processes)
- A process state (new, ready, running, waiting, terminated)
- A program counter and registers (keeps track of the current machine instruction)
- Adress space (memory reserved for the process)
- ...

The operating system is responsible for:

- spawning and terminating processes
- scheduling of processes and deadlock mitigation
- IPC and synchronization

**Scheduling:**

Is a difficult problem since ideally the processes are scheduled in the most efficient way regarding throughput, response time, fairness, priorities and turnaround time but the information
is limited and uncertain (arrival of new processes, early termination, ececution time, I/O operations, interrupts,...).

Scheduling happens when the executing process switches. This can happen non-preemptively (e.g. process is waiting for I/O and yields) or preemptively (e.g. sw-interrupt or CPU quantum used up). Then a context switch happens (switch of process image in the CPU: old process gets saved, new one loaded into CPU) which creates overhead, so changing process too often is ineffective.

#### 1.1.1.1. fork() and other basics

Signature and Headers:

```C
#include <sys/types.h>  // for pid_t
#include <unistd.h>     // for fork(), _exit()
#include <sys/wait.h>   // for wait(), waitpid() (more specific)

pid_t fork(void);
void _exit(int status);
pid_t wait(int *status);

```

The [**fork()**](http://man7.org/linux/man-pages/man2/fork.2.html) function creates a new process by duplicating the calling process.
The calling process is the parent process of the newly created child process.

Return Value:

- 0 for the child
- pid (>0) of the child to parent
- -1 on error

Every Process needs to be [waited](http://man7.org/linux/man-pages/man2/waitpid.2.html) for, in order to free its pid. If a process is not waited for,
the pid won't be released which further limits the maximum number of new processes (zombie processes).

A child process can either be terminated or it can exit on its own.

**NOTE:** Since processes are concurrent and scheduling may keep a process whilst executing an
instruction, the successful completion of an action before another process gets the CPU is not
guaranteed. Atomic operations are operations which take only one clock cycle and therefore  always finish to execute.

#### 1.1.1.2. Example: fork()

```C
#include <stdlib.h>     // for EXIT_SUCESS/FAILURE
#include <stdio.h>      // for fprintf(), perror()
#include <sys/types.h>  // for pid_t
#include <unistd.h>     // for fork(), _exit()
#include <sys/wait.h>   // for wait(), waitpid() (more specific)

int main(int argc, char *argv[]){
    pid_t child;

    if( (child = fork()) == -1){
        perror("fork");
        return EXIT_FAILURE;
    } else if (child == 0) {
        fprintf(stdout, "%d: I bims the child of %d\n", getpid(), getppid());
        // getpid() returns pid of calling process
        // getppid() returns pid of parent
        _exit(EXIT_SUCCESS);     // _exit() is atomic
    }

    fprintf(stdout, "%d: I bims the parent\n", getpid());

    // while( wait(NULL) > 0);     // waits for all children
    waitpid(child, NULL, 0);      // waits only for child

    return EXIT_SUCCESS;
}
```

#### 1.1.1.3. exec()

The [exec family](http://man7.org/linux/man-pages/man3/exec.3.html) of functions replaces the current process image with a new
process image.

[Modifiers:](https://stackoverflow.com/questions/4204915/please-explain-the-exec-function-and-its-family)

- l: parameters as list
- v: parameters in array(vector)
- e: environment modifier
- p: path to search for the new running program

#### 1.1.1.4. Example: exec()

Replaces the current process with ls.

```C
#include <unistd.h>

int main(int argc, char *argv[]){
  execlp("ls", "ls", NULL);
}
```

### 1.1.2. Signals and Signal Handling

[Signals](http://man7.org/linux/man-pages/man7/signal.7.html) are a basic way of exchanging information between processes.
They are similar to software interrupts and are sent on the occurence of an event.
There are signals for all types of events:

- SIGINT (^C)
- SIGALRM
- SIGUSR1
- SIGUSR2
- SIGKILL (can't be caught)
- SIGSTOP (can't be caught)
- ...

#### 1.1.2.1. Synchronous Signals

Occur when the execution of instructions fails unexpectedly (e.g. SIGILL(illegal instruction) or SIGSEGV(illegal adress)) and are sent to the process.

#### 1.1.2.2. Asynchronous Signals

Are sent from beyond of the execution scope of the current process (e.g. SIGINT(^C)).

#### 1.1.2.3. Sending and receiving Signals

```C
#include <sys/types.h>  // for pid_t
#include <signal.h>     // for kill(), signal()
#include <unistd.h>     // alarm(), pause()

// sending signals
int kill(pid_t pid, int sig);

// catching signals
typedef void (*sighandler_t)(int);  // void returning function with int parameter
sighandler_t signal(int signum, sighandler_t handler);

unsigned int alarm(unsigned int seconds);   // sends SIGALRM after seconds seconds
int pause(void);    // waits until any signal arrives

```

#### 1.1.2.4. Example: signal()

Sending SIGALRM to child

The parents sends some SIGALRMs to child, child ignores them and parent kills the
child with SIGINT.

```C
#include <stdlib.h>     // for EXIT_FAILURE/SUCCESS
#include <stdio.h>      // for fprintf(), perror()
#include <unistd.h>     // for fork(), ...
#include <signal.h>     // for signal stuff
#include <sys/wait.h>

void sigalrm_handler(int signo){
    signal(SIGALRM, sigchld_handler);
    // after 1 signal the signal handler needs to be renewed
    printf("%d: received SIGALRM\n", getpid());    // I/O unsafe in sig handlers
}

int main(void){
  pid_t child;
  signal(SIGALRM, sigchld_handler);                // activates signal handler
  switch((child = fork()){
    case -1:
      perror("main: fork");
      exit(1);
      break;
    case 0:
      while(1){ pause(); }                         // just wait for signals
      _exit(EXIT_SUCCESS);
    default:
      for(int i = 0; i < 3; i++){
          alarm(2);                                // sends SIGALRM every 2 seconds
          pause();                                 // catches SIGALRM
          kill(child, SIGALRM);                    // sends SIGALRM to child
      }
      kill(child, SIGINT);                         // kill child
      wait(NULL);
      break;
  }
  return EXIT_SUCCESS;
}
```

#### 1.1.2.5. The hard (and clean) way

```C
#include <sys/types.h>  // for pid_t
#include <signal.h>     // for kill(), signal()
#include <unistd.h>     // alarm(), pause()

// catching signals

volatile sig_atomic_t // atomic integer type

// blocking a set of signals
sigset_t              // structure type represents sets of signals
int sigemptyset(sigset_t *set);  // excludes all signals from signal set
int sigaddset(sigset_t *set, int signo);  // add a specific signal to the set
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
            // block all signals in the mask
// int how: SIG_BLOCK, SIG_UNBLOCK

// catching signals
// struct with signal handler, blocked signals and other options
struct sigaction = {
    .sa_handler = parent_handler,   // specify signal handler
    .sa_flags = 0 // SA_RESTART     // signal handler doesn't need to be renewed
};

int sigfillset(sigset_t *set);  // initializes a signal set -> e.g. starts to block

int sigaction(int sig, const struct sigaction *restrict act,
        struct sigaction *restrict oact);
```

#### 1.1.2.6. Example: sigaction()

Child blocking signal
The parent sends some SIGINTs to child, child ignores them until
a SIGALRM is received, the next SIGINT terminates the child.

```C
#define _POSIX_SOURCE // needed for non C99 standard code like sigset_t
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define err(msg) perror("msg"); _exit(EXIT_FAILURE)

// use an atomic type as a flag to unblock SIGUSR2
volatile sig_atomic_t sig_block = 1;

// prints pid and signal
void confirm(pid_t pid, char* sig){
  printf("%d: received %s\n", pid, sig);
}

// signal handler for parent
void parent_handler(int sig){
  printf("%d: sending signal\n", getpid());
}

// signal handler for child
void child_handler(int sig){
  if(sig == SIGALRM){
    sig_block = 0;
  } else if(sig == SIGINT){
    confirm(getpid(), "SIGINT");
    _exit(0);
  }
}

int main(void){
  pid_t child;
  sigset_t block_mask;

  // initialize signalset to block SIGINT
  sigemptyset(&block_mask);
  if(sigaddset(&block_mask, SIGINT) == EINVAL){
      err("main: sigaddset");
  }
  // block all signals in block_mask
  if(sigprocmask(SIG_BLOCK, &block_mask, NULL) == EINVAL){
      err("main: sigprocmask");
  }

  // create signal handler
  struct sigaction sa = {
    .sa_handler = parent_handler,
    .sa_flags = 0 // SA_RESTART is not supported by _POSIX_SOURCE
  };
  // block every signal during the handler
  sigfillset(&sa.sa_mask);

  // intercept SIGALRM
  if(sigaction(SIGALRM, &sa, NULL) == EINVAL){
    err("main: sigaction");
  }

  // create a child process
  if((child = fork()) == EINVAL){ err("main: fork"); }

  // child
  if(child == 0){
    // overwrite the parents signalhandler
    sa.sa_handler = child_handler;
    // overwrite the signal listeners
    // intercept SIGALRM and SIGINT
    if(sigaction(SIGALRM, &sa, NULL) == EINVAL){
      err("child: sigaction SIGALRM")
    }
    if(sigaction(SIGINT, &sa, NULL) == EINVAL){
      err("child: sigaction SIGINT")
    }

    // send SIGALRM in 15sec to unblock SIGUSR2
    alarm(15);

    // wait for signals
    for(;;){
      pause();
      // if the block_flag is unset unblock SIGINT
      if(sig_block == 0){
        // unblock the previously blocked signals
        sigprocmask(SIG_UNBLOCK, &block_mask, NULL);
        fprintf(stdout, "%d: unblocked signals\n", getpid());
      }
    }
  }

  printf("parent PID: %d\n", getpid());

  for(int i = 1; i =< 4; i++){
    // wait 2 sec (send the alarm to send a signal in 5 sec)
    alarm(2);
    pause();
    kill(child, i % 3 == 0 ? SIGALRM : SIGINT);
  }

  waitpid(child, NULL, 0);

  return EXIT_SUCCESS;
}
```

### 1.1.3. Named and Unnamed Pipes

[Pipes and FIFOs](http://man7.org/linux/man-pages/man7/pipe.7.html) (also known as named pipes) provide a unidirectional
interprocess communication channel.  A pipe has a read end and a
write end. Data written to the write end of a pipe can be read from
the read end of the pipe.

#### 1.1.3.1. Unnamed Pipes

[Unnamed pipes](http://man7.org/linux/man-pages/man2/pipe.2.html) can only be created between processes with a common predecessor.
These pipes are half-duplex, meaning data can only be transmitted from a write end
to a read end.

```C
#include <unistd.h>

int pipe(int pipefd[2]); // creates a pipe
// pipefd[0] is read end, pipefd[1] is write end
int dup(int oldfd);   // duplicates filedescripto, returns new fd
int dup2(int oldfd, int newfd);  // duplicates the oldfd to the newfd
int close(int fd); // used to clese a pipe end
```

#### 1.1.3.2. Example: pipe()

More or less the equivalent to `grep [single keyword] | ls`.

```C
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define err(msg) perror(msg); _exit(EXIT_FAILURE)

int main(int argc, char *argv[]){
  if(argc != 2){
    fprintf(stderr, "Usage: ./%s <keyword>\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  pid_t cpid;
  int pipefd[2];

  // create the pipe
  if(pipe(pipefd) == -1){ perror("pipe"); return EXIT_FAILURE;}
  // fork the process
  if((cpid = fork()) == -1){ err("fork"); }

  if(cpid == 0){
    close(pipefd[0]);   // close read end
    dup2(pipefd[1], 1); // duplicate stdin as write end
    close(pipefd[1]);   // close excess write end

    // call ls
    execlp("ls", "ls", NULL);

    err("exec");
  } else {
    close(pipefd[1]);   // close read end
    dup2(pipefd[0], 0); // dup2 stdout as read end
    close(pipefd[0]);   // close excess write end

    // call grep
    execlp("grep", "grep", argv[1], NULL);

    err("exec");
  }
}
```

#### 1.1.3.3. Named Pipes(FIFOs)

A [FIFO](http://man7.org/linux/man-pages/man3/mkfifo.3.html) is similar to a pipe, but any process
can open it for reading or writing, in like an ordinary
file.  However, it has to be open at both ends simultaneously before
any input or output operations can be done on it. Opening
a FIFO for reading normally blocks until some other process opens the
same FIFO for writing, and vice versa.

```C
#include <sys/types.h>
#include <sys/stat.h>

int mkfifo(const char *pathname, mode_t mode); // path to filename and access rights (0000-7777)
int unlink(const char *pathname); // deletes a file if no other process has the file open
```

#### 1.1.3.4. Example: mkfifo()

The producer file (Server):

**Note:** the program needs to be stopped with ^C.

```C
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // for )_ macros

#define err(msg) perror(msg); _exit(EXIT_FAILURE)

#define FIFO "/tmp/FIFO_NAME"

int main(int argc, char *argv[]){
  FILE *fifo_fp;
  int count;

  if( mkfifo(FIFO, 0666) == -1){ err("mkfifo"); }
  if( (fp_fifo = fopen(FIFO, O_RDONLY)) == NULL){ err("fopen"); }

  for(;;){
    fprintf(fp_fifo, "%d: i bims.\n", getpid());
    sleep(2);
  }

  // closing the pipe (should never get here)
  if( fclose(fp_fifo) == EOF){ err("fclose"); }
  if( unlink(FIFO) == -1){ err("unlink"); }

  return EXIT_SUCCESS;
}
```

The consumer file(Client):

**Note:** the program needs to be stopped with ^C.

```C
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // for )_ macros

#define err(msg) perror(msg); _exit(EXIT_FAILURE)

#define FIFO "/tmp/FIFO_NAME"

int main(int argc, char *argv[]){
  FILE *fifo_fp;
  int count;
  char buf[64] = {0};

  // would throw error if fifo already open
  // if( mkfifo(FIFO, 0666) == -1){ err("mkfifo"); }
  if( (fp_fifo = fopen(FIFO, O_RDONLY)) == NULL){ err("fopen"); }

  for(;;){ // quite inefficient
    if( fscanf(fp_fifo, "%s", buf) ){
      fprintf(stdout, "%d: got '%s' from fifo\n", getpid(), buf);
      buf[0] = 0; // EOF at beginning 'clears' the array
    }
  }

  // closing the pipe (should never get here)
  if( fclose(fp_fifo) == EOF){ err("fclose"); }
  if( unlink(FIFO) == -1){ err("unlink"); }

  return EXIT_SUCCESS;
}
```

Using [select](http://man7.org/linux/man-pages/man2/select.2.html) to read from multiple fifos:

See [here](https://stackoverflow.com/questions/28519119/c-using-select-to-read-from-two-named-pipes-fifo).

### 1.1.4. System V vs. POSIX

[System V IPC](http://man7.org/linux/man-pages/man7/svipc.7.html) is an older shared
memory API.  POSIX shared memory provides a simpler, and better
designed interface; on the other hand POSIX shared memory is somewhat
less widely available (especially on older systems) than System V
shared memory. For different objectives one implementation may be better than the other
(e.g. [System V may not be thread-safe](https://stackoverflow.com/questions/4582968/system-v-ipc-vs-posix-ipc)).

**Note:** Altough both implementations are mentioned below there may not be an example for both
implementations.

### 1.1.5. Semaphores and Shared Memory

Since threads have the same address space, shared memory is only relevant for IPC(not ITC).
Semaphores are a utility for syncronizing processes/threads and may need to be placed in a
shared memory segment to allow different processes to access them.

#### 1.1.5.1. Shared Memory

Shared memory allows processes to communicate information by sharing a region of memory.

#### 1.1.5.2. System V

```C
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

key_t ftok(const char *pathname, int proj_id);
// file at pathname must exist and proj_id must be > 0
// (or just pick a random number like 666 or 42) <- unsafe
int shmget(key_t key, size_t size, int shmflg);
// get a shared memory segment of size size, shmflg specifies permissions

void *shmat(int shmid, const void *shmaddr, int shmflg);
// attack the shared memory segment to the calling process
int shmdt(const void *shmaddr);
// detach the shm segment from the calling process
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
// used to contol(e.g. delete) the shm segment
// a shm segment should be destroyed after all processes detatched from it
```

#### 1.1.5.3. Example: System V shared memory

Since there is no management of the critical region, start the second process after the first.

The producer file:

```C
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define err(msg) perror(msg); _exit(EXIT_FAILURE);

#define SHMSZ 128

int main(void) {
    int shmid;
    key_t key = 5678; // id of shm segment is "5678"
    char *shm;

    // creating the segment
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) { err("shmget"); }

    // attach the segment to our data space
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) { err("shmat"); }

    // deallocate the shm segment when no one is attached anymore
    // could naturally be done later
    if (shmctl(shmid, IPC_RMID, 0) < 0){ err("shmctl"); }

    // storing data in the shm segment
    sprintf(shm, "%d: I bims.", getpid());

    // needs to wait until the other process at least
    // attached the shm or it will be destroyed
    while(*shm != '*'){ sleep(1); }

    // detatch the shm from the data space
    if( shmdt(counter) < 0){ err("shmdt"); }

    return EXIT_SUCCESS;
}
```

The client file:

```C
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define err(msg) perror(msg); _exit(EXIT_FAILURE);

#define SHMSZ 128

int main(void) {
    int shmid;
    key_t key = 5678; // id of shm segment is "5678"
    char *shm;

    // locate the segment
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) { err("shmget"); }

    // attach the segment to our data space
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) { err("shmat"); }

    // read the shm content
    fprintf(stdout, "%d: found shm content\n\n%s\n\n", getpid(), shm);

    // indicate that the other process can finish
    *shm = '*';

    // detatch the shm from the data space
    if( shmdt(counter) < 0){ err("shmdt"); }

    return EXIT_SUCCESS;
}
```

#### 1.1.5.4. POSIX

The POSIX implementation was not introduced in the lecture. See [man](http://man7.org/linux/man-pages/man7/shm_overview.7.html) page.

#### 1.1.5.5. Semaphores

A semaphore(dt. Flaggensignal) is just a fancy unsigned int flag wich can be acessed by multiple
processes if either placed in a file(named semaphore) or in a shared memory segment.

#### 1.1.5.6. System V

System V semaphores are older and more complicated. Each [semaphore](https://docs.oracle.com/cd/E19683-01/806-4125/svipc-65382/index.html) has an associated data structure of type struct semid_ds, defined in `<sys/sem.h>`.

```C
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

key_t ftok(const char *pathname, int proj_id);
// file at pathname must exist and proj_id must be > 0
// (or just pick a random number like 666 or 42) <- unsafe
int semget(key_t key, int nsems, int semflg); // get a semaphore, returns semid
int semctl(int semid, int semnum, int cmd, ...);
// used to control(init, delete) a stuct semid_ds which keeps track of the semaphores
int semop(int semid, struct sembuf *sops, size_t nsops); // change value of semaphore
```

#### 1.1.5.7. Example: System V semaphores

['Simple' semaphore](http://users.cs.cf.ac.uk/Dave.Marshall/C/node26.html#SECTION002650000000000000000) example.

#### 1.1.5.8. POSIX

[POSIX semaphores](http://man7.org/linux/man-pages/man7/sem_overview.7.html) are
provide basically the same functionaliy but with an easier interface.

```C
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

/* DO NOT USE DEPRECATED AND NOT SAFE!! */
// unnamed semaphore
int sem_init(sem_t *sem, int pshared, unsigned int value);
// initializes semaphore at address *sem with the value value
// pshared == 0 -> shared between threads, should be global or on the heap
//         != 0 -> shared between processes, should be located in shared memory

// named semaphore
// needs a name (should start with / e.g. "/semname")
sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value); // see man page

int sem_post(sem_t *sem);
// increments (unlocks the semaphore) and wakes waiting processes/threads
int sem_wait(sem_t *sem);
// decrements (locks) if the value is >0 else waits until value is 0

// unnamed semaphore
int sem_destroy(sem_t *sem); // destroys an unnamed semaphore

// named semaphore
int sem_unlink(const char *name); // removes a named semaphore
```

#### 1.1.5.9. Example: unnamed POSIX semaphore(with shared memory)

```C
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h> // needs -pthred

#define err(msg) perror(msg); _exit(EXIT_FAILURE);

#define SHMSZ sizeof(pid_t)

#define SEMNAME "/mySem"

int main(void) {
    int shmid;
    key_t key = 5678; // id of shm segment is "5678"
    pid_t *shm;
    sem_t *sem;
    pid_t child;

    // creating the segment
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) { err("shmget"); }

    // attach the semaphore
    if( (sem = shmat(shm_id, NULL, 0)) < 0){ err("shmat"); }

    // inits and locks the semaphore
    sem = sem_open(MYSEM, O_CREAT, 0666, 1);

    // attach the segment to our data space
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) { err("shmat"); }

    // semaphore stuff only needed here if
    // other processes could access the shm segment
    if( sem_wait(sem) < 0){ err("sem_wait"); }

    // storing data in the shm segment
    *shm = getpid();

    // release the critical region
    if( sem_post(sem) < 0){ err("sem_post"); }

    if( (child = fork()) < 0){
      sem_destroy(sem); err("fork");
    } else if (child == 0){
      for(;;){  // child tries to trigger parent process
        // acquire the shm
        if( sem_wait(sem) < 0){ err("sem_wait"); }

        fprintf(stdout, "%d: found %d\n", getpid(), *shm);
        *shm = getpid();

        // release the critical region
        if( sem_post(sem) < 0){ err("sem_post"); }
      }
    }

    int triggered = 0;
    for(;;){
      if( sem_wait(sem) < 0){ err("sem_wait"); }

      if( *shm != getpid()){
          if(triggered) {
            fprintf(stdout, "%d: i bim ausgelöst @ %d\n", getpid(), *shm);
            kill(child, SIGKILL);
            waitpid(child, NULL, 0);
            break;
          } else {
            fprintf(stdout, "%d: bitte nicht auslösen @ %d\n", getpid(), *shm);
            triggered++;
          }
          *shm = getpid();
      }
      if( sem_post(sem) < 0){ err("sem_post"); }
    }

    // destroy the semaphore
    sem_unlink(SEMNAME);

    // detatch the shm from the data space
    if( shmdt(counter) < 0){ err("shmdt"); }

    // deallocate the shm segment when no one is attached anymore
    if (shmctl(shmid, IPC_RMID, 0) < 0){ err("shmctl"); }

    return EXIT_SUCCESS;
}
```

## 1.2. Threads

A single process can contain multiple [threads](http://man7.org/linux/man-pages/man7/pthreads.7.html), all of which are
executing the same program.  These threads share the same global
memory (data and heap segments), but each thread has its own stack
(automatic variables).

Threads vs Processes

- Threads have faster context switches since less data has to be copied
- ITC is easier than IPC (e.g. no need for shared memory)
- If one thread blocks(e.g I/O) the others can use the processes CPU time

For the operating system threads separate the resource allocation from scheduling.

- Processes are a unit for resource allocation(CPU time)
- Threads are a unit for the scheduler, since each of the threads is either spawned, ready, running, waiting(blocked) or terminated(finished). When the executing process blocks/waits another one can use the processes CPU time.

**Note:** Only [POSIX threads](http://man7.org/linux/man-pages/man0/pthread.h.0p.html) will be used here.

### 1.2.1. Creating Threads

```C
#include <pthread.h>  // needs -pthread
#include <sys/syscall.h> // for gettid

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
// spawns a new thread wich executes the start routine with the arguments arg

(int type) syscall(SYS_gettid);
// getpid() for threads
pthread_t pthread_self(void);
// similar to getpid() for threads
int pthread_equal(pthread_t t1, pthread_t t2);
// returns 0 if equal

void pthread_cleanup_push(void (*routine)(void *), void *arg);
// routine gets executed when thread is cancelled
// every push needs a pop
void pthread_cleanup_pop(int execute);
// top routine gets popped (and executed if execute != 0)

int pthread_setcancelstate(int state, int *oldstate);
// allows a thread to be cancelled or not
// state -> PTHREAD_CANCEL_ENABLE (DISABLE)
int pthread_cancel(pthread_t thread);
// cancels thread thread (if cancelstate and type allow)

void pthread_exit(void *retval);
// similar to _exit(); but doesn't kill all other threads
// calling _exit() would terminate the process and all threads
int pthread_detach(pthread_t thread);
// automatically releases resources if terminated, doesn't need to be joined
int pthread_join(pthread_t thread, void **retval);
// similar to wait, should be done(zombie threads, like zombie processes)
```

#### 1.2.1.1. Example: pthread_create()

Simply creating a thread:

```C
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h> // compile with -pthread
#include <sys/syscall.h>

#define gettid() syscall(SYS_gettid)

#define handle_error(msg) \
  perror(msg); _exit(EXIT_FAILURE)

#define THREADS 10

void* start_routine(void *arg){
  fprintf(stdout, "%ld: i bims\n", gettid());

  pthread_exit(NULL);
}

int main (void){
  pthread_t tid[THREADS];

  // create THREADS threads
  for(int i = 0; i < THREADS; i++){
    if( pthread_create(&tid[i], NULL, &start_routine, NULL) != 0){
      handle_error("pthread_create");
    }
  }

  // wait for all threads
  for(int i = 0; i < THREADS; i++){
    pthread_join(tid[i], NULL);
  }

  return EXIT_SUCCESS;
}
```

With cancellation and cleanup stack:

```C
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h> // compile with -pthread
#include <sys/syscall.h>

#define gettid() syscall(SYS_gettid)

#define handle_error(msg) \
  perror(msg); _exit(EXIT_FAILURE)

#define handle_error_t(msg) \
  perror(msg); pthread_exit(NULL)

#define THREADS 10

void cleanup_handler(void *fp){
  if (fclose((FILE *) fp) != 0){ handle_error_t("fclose"); }
}

void* start_routine(void *arg){
  // block cancel requests, do something critical (e.g. FILE I/O)
  // update cleanup stack and allow cancellation
  if( pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL) != 0){
    handle_error_t("pthread_setcancelstate");
  }

  pthread_cleanup_push(puts(), "got cancelled");

  if( pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) != 0){
    handle_error_t("pthread_setcancelstate");
  }

  fprintf(stdout, "%ld: i bims\n", gettid());

  // clear cleanup stack
  pthread_cleanup_pop(0);

  pthread_exit(NULL);
}

int main (void){
  pthread_t tid[THREADS];

  // create THREADS threads
  for(int i = 0; i < THREADS; i++){
    if( pthread_create(&tid[i], NULL, &start_routine, NULL) != 0){
      handle_error("pthread_create");
    }
  }

  // cancel a thread if its tid is even
  for(int i = 0; i < THREADS; i++){
    if(tid[i] % 2 == 0){
      if (pthread_cancel(tid[i]) != 0){
        handle_error("pthread_cancel");
      }
    }
  }

  // wait for all threads
  for(int i = 0; i < THREADS; i++){
    pthread_join(tid[i], NULL);
  }

  return EXIT_SUCCESS;
}
```

### 1.2.2. Mutual Exclusion

In order to avoid race conditions (runtime effects may influence the outcome)
mutual exclusion in critical regions is needed.

**Note:** Internally both mutexes and spinlocks may be implemented in a hybrid way
(e.g. first busywaiting is used for a short time and then the thread goes to sleep).

#### 1.2.2.1. Mutexes

Mutexes are an easy way of achieving mutual exclusion in critical regions. A mutex
is either locked or unlocked. If the mutex is already locked by another thread, calling the lock() function shall block(sleep, creating overhead when going to sleep/being woken) until the mutex becomes available.

```C
#include <pthread.h> // needs -pthread

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// initializes mutex 'lock'

int pthread_mutex_lock(pthread_mutex_t *mutex);
// (blocks until mutex is unlocked and) locks the mutex
int pthread_mutex_unlock(pthread_mutex_t *mutex);
// unlocks a locked mutex
```

See [man page](http://man7.org/linux/man-pages/man3/pthread_mutex_destroy.3p.html) for dynamic mutexes.

#### 1.2.2.2. Example: pthread_mutex()

```C
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h> // compile with -pthread
#include <sys/syscall.h>

#define gettid() syscall(SYS_gettid)

#define handle_error(msg) \
  perror(msg); _exit(EXIT_FAILURE)

#define THREADS 10
#define MAX 10000

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int critical_region_thread_count = 0;

void* start_routine(void *arg){
  for(int i = 0; i < MAX; i++){
    pthread_mutex_lock(&lock);
    if(++critical_region_thread_count > 1){
      fprintf(stdout, "%d: %d threads in the critical region\n", \
          gettid(), critical_region_thread_count);
    }
    critical_region_thread_count--;
    pthread_mutex_unlock(&lock);
  }
  fprintf(stdout, "%ld: bim fertig\n", gettid());

  pthread_exit(NULL);
}

int main (void){
  pthread_t tid[THREADS];

  // create THREADS threads
  for(int i = 0; i < THREADS; i++){
    if( pthread_create(&tid[i], NULL, &start_routine, NULL) != 0){
      handle_error("pthread_create");
    }
  }

  // wait for all threads
  for(int i = 0; i < THREADS; i++){
    pthread_join(tid[i], NULL);
  }

  return EXIT_SUCCESS;
}
```

### 1.2.3. Spinlocks

[Spinlocks](http://man7.org/linux/man-pages/man3/pthread_spin_init.3.html) are very similar to mutexes, the difference is that the lock()
operation uses busy waiting until the lock can be acquired. If the critical
region is very small spinlocks may be faster than mutexes and vice versa.

>General note: Most programs should use mutexes instead of spin locks.
> Spin locks are primarily useful in conjunction with real-time
> scheduling policies.

```C
#include <pthread.h>

int pthread_spin_init(pthread_spinlock_t *lock int pshared);
// pshared: PTHREAD_PROCESS_PRIVATE or PTHREAD_PROCESS_SHARED
//          if it is shared, it should be in a shared memory segment

int pthread_spin_lock(pthread_spinlock_t *lock);
// busywaiting until the spinlock can be acquired

int pthread_spin_unlock(pthread_spinlock_t *lock);
// unlocks the previously locked spinlock

int pthread_spin_destroy(pthread_spinlock_t *lock);
// deallocates a spinlock
```

#### 1.2.3.1. Example: pthread_spinlock()

```C
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h> // compile with -pthread
#include <sys/syscall.h>

#define gettid() syscall(SYS_gettid)

#define handle_error(msg) \
  perror(msg); _exit(EXIT_FAILURE)

#define THREADS 10
#define MAX 10000

pthread_spinlock_t lock;

int critical_region_thread_count = 0;

void* start_routine(void *arg){
  for(int i = 0; i < MAX; i++){
    pthread_spinlock_lock(&lock);
    if(++critical_region_thread_count > 1){
      fprintf(stdout, "%d: %d threads in the critical region\n", \
          gettid(), critical_region_thread_count);
    }
    critical_region_thread_count--;
    pthread_spinlock_unlock(&lock);
  }
  fprintf(stdout, "%ld: bim fertig\n", gettid());

  pthread_exit(NULL);
}

int main (void){
  pthread_t tid[THREADS];

  if( pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE) != 0){
    handle_error("spin_destroy");
  }

  // create THREADS threads
  for(int i = 0; i < THREADS; i++){
    if( pthread_create(&tid[i], NULL, &start_routine, NULL) != 0){
      handle_error("pthread_create");
    }
  }

  // wait for all threads
  for(int i = 0; i < THREADS; i++){
    pthread_join(tid[i], NULL);
  }

  if( pthread_spin_destroy(&spinlock) != 0){
    handle_error("spin_destroy");
  }

  return EXIT_SUCCESS;
}
```

### 1.2.4. Condition Variables

Mutexes and spinlocks can only limit access to critical regions where data is shared. Condition
variables allow the implementation of the observer pattern. Condition variables are a critical region
and need to be synchronized using mutexes.

```C
#include <pthread.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// initializes the condition cond

int pthread_cond_init(pthread_cond_t *restrict cond,
    const pthread_condattr_t *restrict attr);
// if attr == NULL this is equal to PTHREAD_COND_INITIALIZER

int pthread_cond_destroy(pthread_cond_t *cond);
// deallocates a initialized condition variable

int pthread_cond_wait(pthread_cond_t *restrict cond,
    pthread_mutex_t *restrict mutex);
// waits until woken by signal/broadcast
// the mutex must be locked, wait() unlocks the mutex again
// if woken wait() reacquires the mutex

int pthread_cond_broadcast(pthread_cond_t *cond);
// wakes all threads waiting for cond
int pthread_cond_signal(pthread_cond_t *cond);
// wakes one thread waiting for cond
```

#### 1.2.4.1. Example: pthread_cond_t

A producer consumer pattern where there are different kinds of consumers
waiting for different events.

The main thread spawns `THREADS` threads and then produces `NUM_ENTRYS` entrys in
a queue. Each consumer only consumes either only even or odd entries. At the end
the producer writes 0 `THREADS` times in the queue. If a consumer reads a 0 it terminates.

**Note:** This program is written in C++ and needs to be compiled with e.g.:

`g++ -Wall -Wextra -pedantic -pthread program.cpp -o program`

```C++
#define _POSIX_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>

#include <queue>
#include <iostream>

#define gettid() syscall(SYS_gettid)

#define err_t(msg) \
  perror(msg); pthread_exit(NULL)

#define err(msg) \
  perror(msg); _exit(EXIT_FAILURE)

#define THREADS 10
#define NUM_ENTRYS 100000

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t even = PTHREAD_COND_INITIALIZER;

std::queue<unsigned> queue;

bool queue_is_empty = true;
bool queue_even = true;

void* even_consumer(void *arg){
  int sum = 0, read;
  while(true){
    pthread_mutex_lock(&lock);
    // pause as long as the queue is empty and even is false
    while(queue_is_empty && !queue_even){
      pthread_cond_wait(&cond, &lock);
    }

    read = queue.front();
    queue.pop();
    queue_is_empty = queue.empty();

    pthread_mutex_unlock(&lock);
    if(read){
      sum += read;
    } else {
      break;
    }
  }
  fprintf(stdout, "%ld: %d\n", gettid(), sum);
  pthread_exit(NULL);
}

void* odd_consumer(void *arg){
  int sum = 0, read;
  while(true){
    pthread_mutex_lock(&lock);
    // pause as long as the queue is empty and even is false
    while(queue_is_empty && queue_even){
      pthread_cond_wait(&cond, &lock);
    }

    read = queue.front();
    queue.pop();
    queue_is_empty = queue.empty();

    pthread_mutex_unlock(&lock);
    if(read){
      sum += read;
    } else {
      break;
    }
  }
  fprintf(stdout, "%ld: %d\n", gettid(), sum);
  pthread_exit(NULL);
}

int main (void){
  pthread_t tid[THREADS];

  for(int i = 0; i < THREADS; i++){
    if(i % 2 == 0){
      if( pthread_create(&(tid[i]), NULL, &even_consumer, NULL) != 0){
        err("pthread_create");
      }
    } else {
      if( pthread_create(&(tid[i]), NULL, &odd_consumer, NULL) != 0){
        err("pthread_create");
      }
    }
  }

  for(int i = 1; i <= NUM_ENTRYS; i++){
    pthread_mutex_lock(&lock);
    queue.push(i);

    queue_even = i % 2 == 0;
    queue_is_empty = false;

    pthread_signal(queue_even);
    pthread_signal(queue_is_empty);

    pthread_mutex_unlock(&lock);
  }
  for(int i = 0; i < THREADS; i++){
    pthread_mutex_lock(&lock);
    queue.push(0);

    queue_even = queue_even ? false : true;
    queue_is_empty = false;

    pthread_signal(queue_even);
    pthread_signal(queue_is_empty);

    pthread_mutex_unlock(&lock);
  }

  for(int i = 0; i < THREADS; i++){
    pthread_join(tid[i], NULL);
  }

  if(pthread_cond_destroy(&empty) != 0){ err("cond_destroy"); }
  if(pthread_cond_destroy(&even) != 0){ err("cond_destroy"); }
  return EXIT_SUCCESS;
}
```

## 1.3. Usage Patters

### 1.3.1. Server - Clients

#### 1.3.1.1. Example: Task 4.1

### 1.3.2. Producer - Consumers

#### 1.3.2.1. Example: Task 6

## 1.4. Task Overview

[Task Overview](tasks.md)

### 1.4.1. Solutions

#### 1.4.1.1. Task 1

[Specifications](.)

[Solution](jfkblatt1)

#### 1.4.1.2. Task 2

[Specifications](.)

[Solution](jfkblatt2)

#### 1.4.1.3. Task 3

[Specifications](.)

[Solution](jfkblatt3)

#### 1.4.1.4. Task 4

[Specifications](.)

[Solution](jfkblatt4)

#### 1.4.1.5. Task 5

[Specifications](.)

[Solution](jfkblatt5)

#### 1.4.1.6. Task 6

[Specifications](.)

[Solution](jfkblatt6)

#### 1.4.1.7. Task 7

[Specifications](.)

[Solution](jfkblatt7)

#### 1.4.1.8. Task 8

[Specifications](.)

[Solution](jfkblatt8)

#### 1.4.1.9. Task 9

[Specifications](.)

[Solution](jfkblatt9)

#### 1.4.1.10. Task 10

[Specifications](.)

[Solution](jfkblatt10)

## 1.5. Further Reading

- [Advanced Linux Programming](http://web.archive.org/web/20150511104538/http://www.advancedlinuxprogramming.com/alp-folder/) ([IPC only](http://web.archive.org/web/20150524005553/http://advancedlinuxprogramming.com/alp-folder/alp-ch05-ipc.pdf))
- [Programming in C - UNIX System Calls and Subroutines using C.](http://users.cs.cf.ac.uk/Dave.Marshall/C/)
