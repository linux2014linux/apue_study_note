#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include <errno.h>

#define ERR_DUMP(ret) printf("errno:%d, %s\n", ret, strerror(ret))

static int g_fd = -1;

int main(int argc, char* argv[]) {

    extern int errno;

    if (2 != argc) {
        printf("usage: ./output <filename>\n");
        return 0;
    }

    // create pause thread
    pid_t pid;
    pid = getpid();
    printf("pid:%d\n", pid);

    // open file and write
    g_fd = open(argv[1], O_CREAT | O_EXCL | O_RDWR, 0777);
    if (-1 == g_fd) {
        printf("open file failed.\n");
        ERR_DUMP(errno);
        return 0;
    }
    // mv: old_name->new_name,仅仅是文件的名字变化,写入依旧是正常的
    // rm: old_name,entry被删除,inode和block依然正常,但proc目录fd的链接将会写向tmp目录,待进程关闭后文件该tmp文件被清空
    printf("mv %s <new_name>\n", argv[1]);
    printf("rm %s\n", argv[1]);

    long long num = 0;
    char buf[128] = {0};
    for ( ; ; ) {
        snprintf(buf, 128, "%lld\n", num);

        int w_ret = write(g_fd, buf, strlen(buf));
        if (0 > w_ret) {
            printf("%s write error.\n", buf);
        } else {
            num++;
        }
        usleep(1000*100);
    }

    close(g_fd);
    return 0;
}
