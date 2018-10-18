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
        sleep(10);
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
    pid = getpid();
    printf("pid is: %d\n", pid);
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
