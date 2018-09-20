#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static jmp_buf env_alrm;
static void sig_alrm(int signo) {
    longjmp(env_alrm, 1);
}

unsigned int my_sleep(unsigned int nsecs) {
    if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
        return nsecs;
    }

    // pause之前,可能发出的alarm已经执行完毕,使用setjmp避免pause死等
    if (setjmp(env_alrm) == 0) {
        alarm(nsecs);
        pause();
    }

    return alarm(0);
}

static void sig_int(int signo) {
    int i, j;
    volatile int k;
    printf("\nsig_int starting\n");
    for (i = 0; i < 300000; i++) {
        for (j = 0; j < 4000; j++) {
            k += i * j;
        }
    }
    printf("sig_int finished\n");
}

int main(int argc, char* argv[]) {
    unsigned int unslept;
    if (signal(SIGINT, sig_int) == SIG_ERR) {
        printf("install sig_int failed\n");
        exit(0);
    }

    unslept = my_sleep(10);
    printf("my_sleep returned: %u\n", unslept);

    return 0;
}
