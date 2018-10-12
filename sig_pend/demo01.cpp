// 未决信号

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

static void sig_quit(int signo) {
    printf("caught SIGQUIT\n");
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR) {
        printf("can't reset SIGQUIT. error: %d, %s\n", errno, strerror(errno));
    }
}

int main(int argc, char* argv[]) {
    pid_t pid;
    sigset_t newmask, oldmask, pendmask;

    pid = getpid();
    printf("pid: %d\n", pid);

    // 设置SIGQUIT的信号处理函数
    if (signal(SIGQUIT, sig_quit) == SIG_ERR) {
        printf("error: %d, %s\n", errno, strerror(errno));
    }

    // 屏蔽SIGQUIT信号
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        printf("error: %d, %s\n", errno, strerror(errno));
    }

    // 休眠30s,此时可向改进程发送SIGQUIT信号
    printf("kill -QUIT %d\n", pid);
    sleep(30);

    // 休眠结束后,获取未决信号
    if (sigpending(&pendmask) < 0) {
        printf("error: %d, %s\n", errno, strerror(errno));
    }
    if (sigismember(&pendmask, SIGQUIT)) {
        printf("SIGQUIT pending\n");
    }

    // 重置屏蔽信号集
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        printf("error: %d, %s\n", errno, strerror(errno));
    }
    printf("SIGQUIT unblocked\n");

    // 此时再次发送SIGQUIT信号将产生core文件
    printf("kill -QUIT %d\n", pid);
    sleep(30);

    return 0;
}
