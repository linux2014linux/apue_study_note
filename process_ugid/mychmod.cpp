#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    struct stat f_buf;
    if (stat(argv[1], &f_buf) < 0) {
        printf("stat failed.\n");
    }
    if (chmod(argv[1], (f_buf.st_mode) | S_ISUID)) {
        printf("stat failed.\n");
    }
    printf("change I_ISUID succ\n");

    return 0;
}
