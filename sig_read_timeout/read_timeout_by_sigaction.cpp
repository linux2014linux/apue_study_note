#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAXLINE 1024

extern int errno;

static void sig_alrm(int);
static void sig_usr(int);

int main(int argc, char* argv[]) {

    int n;
    char line[MAXLINE];
    struct sigaction act, oact;
    pid_t pid;

    // 设置SIGUSR1为不重启动类型,当接收到SIGUSR1信号时,read将立即返回
    act.sa_handler = sig_usr;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0 | SA_INTERRUPT;
    sigaction(SIGUSR1, &act, &oact);

    pid = getpid();
    printf("pid=%d\n", pid);

    // 设置ALARM为默认,默认时重启动的类型,因此当超时时,read并不会返回
    if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
        printf("signal error\n");
        return 0;
    }
    alarm(3);

    memset(line, '0', MAXLINE);
    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0) {
        printf("read error: %d-%s\n", errno, strerror(errno));
        return 0;
    }
    alarm(0);
    
    write(STDOUT_FILENO, line, n);
    exit(0);
}

static void sig_alrm(int signo) {
    printf("read timeout\n");

    return;
}

static void sig_usr(int signo) {

    if (signo == SIGUSR1) {
        printf("receive SIGUSR1\n");
    } else if (signo == SIGUSR2) {
        printf("receive SIGUSR2\n");
    } else {
        printf("receive signal %d\n", signo);
    }

    return;
}
