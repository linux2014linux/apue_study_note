#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>

static void my_alarm(int signo) {
    struct passwd *rootptr;
    printf("in signal handler\n");
    // 此处访问了标准IO库的全局变量,与主流程会产生问题
    if ((rootptr = getpwnam("root")) == NULL) {
        printf("getpwnam root error\n");
    }
    alarm(1);
}

int main(int argc, char* argv[]) {

    struct passwd *ptr;
    // 进程在发送alrm之后,处于休眠状态,即因为访问了全局共享变量
    signal(SIGALRM, my_alarm);
    alarm(1);
    for ( ; ; ) {
        if ((ptr = getpwnam("lt")) == NULL) {
            printf("getpwnam error");
        }

        // 因为getpwnam内部共享全局变量,因此此处pw_name有可能不是lt
        if (strcmp(ptr->pw_name, "lt") != 0) {
            printf("return vqalue corrupted! pw_name = %s\n", ptr->pw_name);
        } else {
            printf("return vqalue corrupted! pw_name = %s\n", ptr->pw_name);
        }
        /**
          此处休眠时,对于全局变量的修改通常不会产生问题.而当不休眠时,可能会出现问题
          比如进程一致处于S状态,死锁
         */
        // sleep(1);
    }

    return 0;
}
