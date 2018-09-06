#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    printf("fork before");
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        // child
        ;
    } else if (pid > 0) {
        // parent
        ;
    } else {
        perror("fork error");
    }

    return 0;
}
