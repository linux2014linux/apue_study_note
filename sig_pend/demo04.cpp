#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

static void sig_handler(int sig_no) {
    printf("caught SIGINT 1\n");
    printf("休眠30s,可发送SIGINT信号,由于信号处理不是队列处理,所以不会排队处理\n");
    sleep(30);
    printf("caught SIGINT 2\n");

    return;
}

int main(int argc, char* argv[]) {
    pid_t pid;
    sigset_t newmask, oldmask, pendmask;

    pid = getpid();
    printf("kill -INT %d\n", pid);

    signal(SIGINT, sig_handler);
    kill(pid, SIGINT);

    return 0;
}
