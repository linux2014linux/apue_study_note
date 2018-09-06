#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/inotify.h>

#define BUFSZ   16384

static void errexit(const char *s) {
    fprintf(stderr, "%s\n", s);
    exit(1);
}

int main(int argc, char* argv[]) {
    int ifd, wd, i, n;
    char buf[BUFSZ];

    if (2 != argc) {
        printf("usage: file_del_monitor <filename>");
        return 0;
    }

    ifd = inotify_init();
    if (ifd < 0)
        errexit("cannot obtain an inotify instance");

    char* filename = argv[1];

    wd = inotify_add_watch(ifd, filename, IN_MODIFY|IN_CREATE|IN_DELETE|IN_MOVE);
    if (wd < 0)
        errexit("cannot add inotify watch");

    int outter_idx = 0;
    while (1) {
        n = read(ifd, buf, sizeof(buf));
        if (n <= 0) {
            errexit("read problem");
        }
        printf("Some event happens, len:%d\n", n);

        i = 0;
        int inner_idx = 0;
        while (i < n) {
            struct inotify_event *ev;

            ev = (struct inotify_event *)(&buf[i]);
            printf("%d name:%s\n", inner_idx, ev->name);
            /*
            if (ev->len) {
                if (ev->mask & IN_CREATE) {
                    printf("file %s is created\n", ev->name);
                } else if (ev->mask & IN_DELETE) {
                    printf("file %s is deleted\n", ev->name);
                } else if (ev->mask & IN_MODIFY) {
                    printf("file %s is modify\n", ev->name);
                } else if (ev->mask & IN_MOVE) {
                    printf("file %s is move\n", ev->name);
                } else {
                    printf("file %s is modify\n", ev->name);
                }
            }
            */
            i += sizeof(struct inotify_event) + ev->len;
        }
        printf("%d---\n", outter_idx++);
    }
}
