#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    int fd;
    size_t size;
    char path[] = "test.fifo";
    (void) umask(0);

    if ((fd = mknod(path, S_IFIFO | 0666, 0)) < 0) {
       printf("Can\'t create fifo.\n");
       exit(-1);
    }

    if ((fd = open(path, O_WRONLY)) < 0) {
       printf("Can\'t open fifo.\n");
       exit(-1);
    }

    size = write(fd, "Hello, world!", 14);

    if (size != 14) {
       printf("Can\'t write all string to fifo.\n");
       exit(-1);
    }

    return 0;
}

