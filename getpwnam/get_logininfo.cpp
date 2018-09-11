#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <pwd.h>

#include <unistd.h>
#include <errno.h>

void dump_passwd(struct passwd *pwd) {
    if (NULL != pwd) {
        printf("name:%s, passwd:%s, uid:%d, gid:%d, uinfo:%s, homedir:%s, shell:%s\n", pwd->pw_name, pwd->pw_passwd, pwd->pw_uid, pwd->pw_gid, pwd->pw_gecos, pwd->pw_dir, pwd->pw_shell);
    }

    return;
}

int main(int argc, char* argv[]) {
    extern int errno;

    if (2 != argc) {
        printf("usage: ./get_logininfo <username>\n");
        return 0;
    }
    char *user = argv[1];
 
    struct passwd *pwd = NULL;
    pwd = getpwnam(user);
    if (NULL == pwd) {
        printf("%d, %s\n", errno, strerror(errno));
        return 0;
    }
    dump_passwd(pwd);

    return 0;
}
