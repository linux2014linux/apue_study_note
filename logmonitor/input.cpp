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
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

static void* t_file_exist(void* argv) {
    char* filename = (char*)argv;
    for( ; ; ) {
        if (access(filename, F_OK) == -1) {
            // 文件已经被mv或者delete,此时关闭旧的fd,重新打开文件
            printf("%s not exist\n", filename);
            if (g_fd != -1) {
                printf("%d is closed\n", g_fd);
                close(g_fd);
                g_fd = open(filename, O_CREAT | O_EXCL | O_RDWR, 0777);
                if (-1 == g_fd) {
                    printf("open file failed.\n");
                    ERR_DUMP(errno);
                    exit(0);
                }
                printf("%s is reopen, fd:%d\n", filename, g_fd);
                pthread_mutex_unlock(&g_mutex);
            }
        } else {
            // 文件存在时,休眠3s
            sleep(3);
        }
    }

    return NULL;
}

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
    printf("ln /proc/%d/fd/%d\n", pid, g_fd);

    // file monitor thread
    // mv: 文件名称变更后,如不重新打开文件,写入的内容转向新文件.此时重新打开,让其写新文件,避免旧文件过大
    // delete: 删除后,内容会一直写入/proc/<pid>/fd/<fd>中.此时将其关闭,避免/proc目录暴增,重新打开文件写入新文件中
    pthread_t pt;
    int t_file_ret = pthread_create(&pt, NULL, t_file_exist, (void*)(argv[1]));
    if (0 != t_file_ret) {
        ERR_DUMP(t_file_ret);
        return 0;
    }

    long long num = 0;
    char buf[128] = {0};
    for ( ; ; ) {
        snprintf(buf, 128, "%lld\n", num);

        int w_ret = write(g_fd, buf, strlen(buf));
        if (0 > w_ret) {
            printf("%s write error.\n", buf);
            pthread_mutex_lock(&g_mutex);
        } else {
            num++;
        }
        usleep(1000*500);
    }

    close(g_fd);
    return 0;
}
