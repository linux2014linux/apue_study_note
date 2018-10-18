#include <sys/types.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static jmp_buf env;

void my_handler(int signo) {
  printf("SIGUSR1 received!\n");
  siglongjmp(env, 1);
  return;
}


int main(int argc, char* argv[]) {
  extern int errno;

  pid_t pid;
  pid = getpid();
  printf("pid is: %d\n", pid);
  if (signal(SIGUSR1, my_handler) == SIG_ERR) {
    printf("error: %d, %s\n", errno, strerror(errno));
  }

  if (sigsetjmp(env, 1) != 0) {
    printf("siglongjmp executed\n");
  }

  for( ; ; ) {
    pause();
  }

  return 0;
}
