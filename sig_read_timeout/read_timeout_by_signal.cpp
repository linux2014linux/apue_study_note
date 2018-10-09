#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1024

static void sig_alrm(int);

int main(int argc, char* argv[]) {

    int n;
    char line[MAXLINE];

    if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
        printf("signal error\n");
        return 0;
    }

    alarm(3);
    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0) {
        printf("read error\n");
        return 0;
    }
    alarm(0);
    
    write(STDOUT_FILENO, line, n);
    exit(0);
}

static void sig_alrm(int signo) {
    printf("read timeout\n");
    exit(0);
}

