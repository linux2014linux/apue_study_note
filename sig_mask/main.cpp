// 信号屏蔽Demo演示

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void my_handler(int signo) {
    if (signo == SIGUSR1) {
        printf("SIGUSR1 received!\n");
    } else if (signo == SIGUSR2) {
        printf("SIGUSR2 received!\n");
    } else {
        printf("signal %d received\n", signo);
    }

    return;
}


int main(int argc, char* argv[]) {
    extern int errno;
    pid_t pid;
    sigset_t sigset;
    sigset_t sig_mask_set;

    pid = getpid();
    /**
     添加需要屏蔽的信号
     */
    sigaddset(&sigset, SIGUSR2);
    if (sigprocmask(SIG_BLOCK, &sigset, &sig_mask_set) < 0) {
        printf("error: %d, %s\n", errno, strerror(errno));
    }

    /**
     校验刚才添加的信号是否被屏蔽
     SIG_BLOCK 屏蔽sigset中的信号
     SIG_UNBLOCK 取消sigset中的信号屏蔽
     SIG_SETMASK 用sigset替换原有的屏蔽信号集
     */
    sigemptyset(&sigset);
    if (sigprocmask(SIG_BLOCK, &sigset, &sig_mask_set) < 0) {
        printf("error: %d, %s\n", errno, strerror(errno));
    }
    if (sigismember(&sig_mask_set, SIGUSR2)) {
        printf("SIGUSR2 is masked. You can test by kill -USR2 %d\n", pid);
    }

    // kill -USR1 pid
    if (signal(SIGUSR1, my_handler) == SIG_ERR) {
        printf("error: %d, %s\n", errno, strerror(errno));
    }

    // kill -USR2 pid
    if (signal(SIGUSR2, my_handler) == SIG_ERR) {
        printf("error: %d, %s\n", errno, strerror(errno));
    }

    for( ; ; ) {
        pause();
    }

    return 0;
}
