#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/inotify.h>

#define WD_NUM 3
#define BUFSZ   16384
#define EVENT_NUM 16
#define MAX_BUF_SIZE 1024

struct wd_name {
    int wd;
    char * name;
};
struct wd_name wd_array[WD_NUM];

char * event_array[] = {
    "File was accessed", 
    "File was modified",
    "File attributes were changed",
    "writtable file closed",
    "Unwrittable file closed",
    "File was opened",
    "File was moved from X",
    "File was moved to Y",
    "Subfile was created",
    "Subfile was deleted",
    "Self was deleted",
    "Self was moved",
    "",
    "Backing fs was unmounted",
    "Event queued overflowed",
    "File was ignored"
};
     
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("usage: ./monitro_test <filename> [ filename ...]");
        return 0;
    }
     
    int fd;
    fd = inotify_init();
    if (fd < 0) {
        printf("Fail to initialize inotify.\n");
        exit(-1);
    }

    // add monitor for files
    int wd;
    int i = 0;
    for (i=0; i<argc-1; i++) {
        wd_array[i].name = argv[i+1];
        wd = inotify_add_watch(fd, wd_array[i].name, IN_ALL_EVENTS);
        if (wd < 0) {
            printf("Can't add watch for %s.\n", wd_array[i].name);
            exit(-1);
        }
        wd_array[i].wd = wd;
    }

    // monitor all status. when no event happen, read will block
    char buffer[1024];
    char * offset = NULL;
    struct inotify_event * event;
    int r_len, extra_event_size;
    char strbuf[16];
    while(r_len = read(fd, buffer, MAX_BUF_SIZE)) {
        offset = buffer;
        // printf("Some event happens, r_len = %d.\n", r_len);
        event = (struct inotify_event *)buffer;
        while (((char *)event - buffer) < r_len) {

            // file type
            if (event->mask & IN_ISDIR) {
                memcpy(strbuf, "Direcotory", 11);
            } else {
                memcpy(strbuf, "File", 5);
            }
            printf("Object type: %s\n", strbuf);

            // file name
            for (i=0; i<WD_NUM; i++) {
                if (event->wd != wd_array[i].wd) {
                    continue;
                }
                printf("Object name: %s\n", wd_array[i].name);
                break;
            }

            // event mask
            printf("Event mask: %08X, ", event->mask);
            for (i=0; i<EVENT_NUM; i++) {
                if (event_array[i][0] == '\0') continue;
                if (event->mask & (1<<i)) {
                    printf("%s\n", event_array[i]);
                }
            }
            printf("\n");

            extra_event_size = sizeof(struct inotify_event) + event->len;
            // next event
            event = (struct inotify_event *)(offset + extra_event_size); 
            offset += extra_event_size;
        }
    }
}
