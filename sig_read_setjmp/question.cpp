#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1024

static void sig_alrm(int);
static jmp_buf env_alrm;

int main(int argc, char* argv[]) {
    
    int n;
    char line[MAXLINE];

    if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
        printf("signal error\n");
        return 0;
    }

    /**
     为何不会产生在setjmp和longjmp之间的死循环
     */
    if (setjmp(env_alrm) != 0) {
        printf("read timeout\n");
        // return 0;
    }

    alarm(5);
    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0) {
        printf("read error\n");
    }
    alarm(0);

    write(STDOUT_FILENO, line, n);
    return 0;
}

static void sig_alrm(int signo) {
    longjmp(env_alrm, 1);
}
