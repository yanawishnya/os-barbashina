#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>

int main() {
    int fd;
    size_t size;
    char path[] = "test.fifo";
    char resstring[14];

    if ((fd = open(path, O_RDONLY)) < 0) {
       printf("Can\'t open fifo.\n");
       exit(-1);
    }

    size = read(fd, resstring, 14);

    if (size < 0) {
       printf("Can\'t read all strings.\n");
       exit(-1);
    }

    printf("RESULT: \n\tRESSTRING = %s\n", resstring);

    if (close(fd) < 0) {
       printf("Can\'t close fifo.\n");
    }

    return 0;
}

