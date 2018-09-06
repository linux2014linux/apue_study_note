#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {

    extern int errno;
    pid_t pid = fork();
    if (pid < 0) {
        printf("fork error!");
    } else if (pid > 0) {
        // parent
        ;
    } else {
        // child
        // exec error: Bad address
        // int ret = execl("/bin/ls", "ls", "-l", "../", "-tr");

        // ok
        // int ret = execl("/bin/ls", "ls", "-l", "../", 0);

        // exec error: Bad address
        // int ret = execl("/bin/ls", "ls", "-l", "../", 23);

        // ls: cannot access  -t ../: No such file or directory
        int ret = execl("/bin/ls", "ls", "-l", " -t ../", (char*)0);
        if (ret < 0) {
            printf("exec error: %s\n", strerror(errno));
        }
        return 0;
    }

    return 0;
}
