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

#define REST_TIME 1
#define ERR_DUMP(ret) printf("errno:%d, %s\n", ret, strerror(ret))

static int g_fd = -1;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

static void* t_file_exist(void* argv) {
    char* filename = (char*)argv;
    static int open_fail = 0;
    for( ; ; ) {
        if (access(filename, F_OK) == -1) {
            // 文件不存在时,关闭之前的fd,适当休眠等待
            if (g_fd != -1) {
                close(g_fd);
                g_fd = -1;
            }
            sleep(REST_TIME);
        } else {
            if (-1 == g_fd) {
                // 文件存在,将未打开.打开文件
                g_fd = open(filename, O_RDONLY, 0777);
                if (-1 == g_fd) {
                    // 打开失败,休眠等待,尝试3次直接退出
                    if (open_fail > 2) {
                        printf("尝试打开文件失败.\n");
                        exit(0);
                    }
                    open_fail++;
                    sleep(REST_TIME);
                } else {
                    // 打开成功,放开锁主线程继续读文件
                    open_fail = 0;
                    pthread_mutex_unlock(&g_mutex);
                }
            } else {
                // 文件存在,且已打开.监控线程休眠等待
                sleep(REST_TIME);
            }
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

    pid_t pid;
    pid = getpid();
    printf("pid:%d\n", pid);
    g_fd = open(argv[1], O_RDONLY, 0777);
    if (-1 == g_fd) {
        printf("open file failed.\n");
        ERR_DUMP(errno);
        return 0;
    }
    printf("/proc/%d/fd/%d\n", pid, g_fd);

    pthread_t pt;
    int t_file_ret = pthread_create(&pt, NULL, t_file_exist, (void*)(argv[1]));
    if (0 != t_file_ret) {
        ERR_DUMP(t_file_ret);
        return 0;
    }

    long long num = 0;
    char buf[128] = {0};
    for ( ; ; ) {
        memset(buf, 0, 128);
        int r_ret = read(g_fd, buf, sizeof(buf) - 1);
        if (0 < r_ret) {
            // 读到内容输出
            printf("%s", buf);
        } else if (0 > r_ret) {
            // 读出错等待
            printf("%s read error.wait new file created.\n", argv[1]);
            pthread_mutex_lock(&g_mutex);
        } else {
            // 读到末尾,短暂等待
            ERR_DUMP(errno);
            printf("end file, wait new content\n");
            usleep(1000*500);
        }
    }

    close(g_fd);
    return 0;
}
