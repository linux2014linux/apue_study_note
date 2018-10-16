#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void my_handler(int signo) {
    if (signo == SIGUSR1) {
        printf("SIGUSR1 start\n");
        sleep(10);
        printf("SIGUSR1 end\n");
    } else if (signo == SIGUSR2) {
        printf("SIGUSR2 start\n");
        printf("SIGUSR2 end\n");
    } else {
        printf("signal %d received\n", signo);
    }

    return;
}


int main(int argc, char* argv[]) {
    extern int errno;

    pid_t pid;
    pid = getpid();
    printf("kill -USR1 %d\n", pid);
    printf("kill -USR2 %d\n", pid);

    struct sigaction sga, osga;
    sga.sa_handler = my_handler;
    sigemptyset(&sga.sa_mask);
    sigaddset(&(sga.sa_mask), SIGUSR2);
    sigaction(SIGUSR1, &sga, &osga);

    if (signal(SIGUSR2, my_handler) == SIG_ERR) {
        printf("error: %d, %s\n", errno, strerror(errno));
    }

    for( ; ; ) {
        pause();
    }

    return 0;
}
