#define _POSIX_SOURCE // needed for non C99 standard code like sigset_t
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/errno.h>

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
    // unsafe in handlers
    printf("%d: unblocked SIGUSR2\n", getpid());
  } else if(sig == SIGUSR1){
    confirm(getpid(), "SIGUSR1");
  } else if(sig == SIGUSR2){
    confirm(getpid(), "SIGUSR2");
    // use atomic call in handler
    _exit(0);
  }
}

int main(void){
  pid_t child;
  sigset_t block_mask;

  // initialize signalset to block SIGUSR2
  sigemptyset(&block_mask);
  if(sigaddset(&block_mask, SIGUSR2) == EINVAL){
    perror("main: sigaddset");
    return EXIT_FAILURE;
  }
  // block all signals in block_mask
  if(sigprocmask(SIG_BLOCK, &block_mask, NULL) == EINVAL){
    perror("main: sigprocmask");
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
    perror("main: couldn't handle SIGALRM");
  }
 
  // create a child process
  if((child = fork()) == EINVAL){
    perror("main: fork");
    return EXIT_FAILURE;
  }

  // child
  if(child == 0){
    // overwrite the parents signalhandler
    sa.sa_handler = child_handler;
    // overwrite the signal listeners
    // intercept SIGALRM, SIGUSR1 and SIGUSR2
    if(sigaction(SIGUSR1, &sa, NULL) == EINVAL){
      perror("main: couldn't handle sigusr1");
    } 
    if(sigaction(SIGUSR2, &sa, NULL) == EINVAL){
      perror("main: couldn't handle sigusr2");
    }
    if(sigaction(SIGALRM, &sa, NULL) == EINVAL){
      perror("main: couldn't handle SIGALRM");
    }

    // print PID
    printf("child PID: %d\n", getpid());

    // send SIGALRM in 15sec to unblock SIGUSR2
    alarm(15);

    // wait for signals
    for(;;){
      pause();
      // if the block_flag is unset unblock SIGUSR2
      if(!sig_block){
        // unblock the previously blocked signals
        sigprocmask(SIG_UNBLOCK, &block_mask, NULL);
      }
    }
  }

  // parent

  // print PID
  printf("parent PID: %d\n", getpid());

  for(int i = 0; i < 4; i++){
    // wait 5 sec (send the alarm to send a signal in 5 sec)
    alarm(5);
    pause();

    // send signal
    kill(child, i < 3 ? SIGUSR1 : SIGUSR2);
    // kill(child, SIGUSR2);
  }

  // wait for child
  waitpid(child, NULL, 0);

  return EXIT_SUCCESS;
}
