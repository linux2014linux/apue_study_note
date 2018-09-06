#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    int fd = open("/etc/subuid-", O_RDONLY, 0666);
    extern int errno;

    if (-1 == fd) {
        printf("open /etc/subuid- failed\n");
        printf("%d, %s\n", errno, strerror(errno));
        return 0;
    }
    char buf[1024] = {0};
    read(fd, buf, 1024);
    printf("%s\n", buf);
    close(fd);

    return 0;
}
