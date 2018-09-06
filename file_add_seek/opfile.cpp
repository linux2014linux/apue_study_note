#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char* argv[]) {

    std::string filename = "/home/lt/Study/cpp_code/opfile.data";
    int fd = open(filename.c_str(), O_RDWR | O_APPEND, 0777);
    if (-1 == fd) {
        std::cout << filename << " opened fail." << std::endl;
        return 0;
    }

    // read
    char buf[1024] = {0};
    lseek(fd, 5, SEEK_SET);
    read(fd, buf, 5);
    std::cout << buf << std::endl;

    lseek(fd, 0, SEEK_SET);
    read(fd, buf, 5);
    std::cout << buf << std::endl;
    write(fd, "zzz", 3);

    close(fd);

    return 0;
}
