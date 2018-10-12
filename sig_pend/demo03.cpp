#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

static void sig_handler(int sig_no) {
    switch(sig_no) {
        case SIGINT:
            printf("caught SIGINT\n");
            break;
        case SIGQUIT:
            printf("caught SIGQUIT\n");
            break;
        case SIGABRT:
            printf("caught SIGABRT\n");
            break;
        default:
            printf("caught othre signal\n");
            break;
    }

    return;
}

int main(int argc, char* argv[]) {
    pid_t pid;
    sigset_t newmask, oldmask, pendmask;

    pid = getpid();
    printf("pid: %d\n", pid);

    signal(SIGINT, sig_handler);

    // 添加屏蔽信号
    sigaddset(&newmask, SIGINT);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        printf("error: %d, %s\n", errno, strerror(errno));
    }

    // 休眠结束后,获取未决信号
    if (sigpending(&pendmask) < 0) {
        printf("error: %d, %s\n", errno, strerror(errno));
    }

    kill(pid, SIGINT);
    kill(pid, SIGINT);
    kill(pid, SIGINT);
    sigpending(&pendmask);

    // 取消屏蔽
    sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
    // sigprocmask(SIG_SETMASK, &oldmask, NULL);

    if (sigismember(&pendmask, SIGINT)) {
        printf("SIGINT came\n");
    }

    return 0;
}
