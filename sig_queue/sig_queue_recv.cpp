#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 由于glibc的bug,此处需要将gettid如下
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)

void handler(int signo) {
  if (signo == SIGINT) {
    printf("%ld ", gettid());
    printf("recv SIGINT signo=%d\n", signo);
  } else if (signo == SIGRTMIN) {
    printf("%ld ", gettid());
    printf("recv SIGRTMIN signo=%d\n", signo);
  } else if (signo == SIGUSR1) {
    sigset_t s;
    sigemptyset(&s);
    sigaddset(&s, SIGINT);
    sigaddset(&s, SIGRTMIN);
    sigprocmask(SIG_UNBLOCK, &s, NULL);
    printf("%ld ", gettid());
    printf(" SIGUSR1 unblock signal.\n");
  } else {
    printf("%ld ", gettid());
    printf("recv other signo=%d\n", signo);
  }

  return;
}

int main(int argc, char* argv[]) {
  extern int errno;

  pid_t pid = getpid();
  printf("%d\n", pid);

  struct sigaction act;
  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  sigset_t s;
  sigemptyset(&s);
  sigaddset(&s, SIGINT);
  sigaddset(&s, SIGRTMIN);
  sigprocmask(SIG_BLOCK, &s, NULL);
  sigaction(SIGINT, &act, NULL);
  sigaction(SIGRTMIN, &act, NULL);
  sigaction(SIGUSR1, &act, NULL);

  for( ; ; ) {
    pause();
  }

  return 0;
}
