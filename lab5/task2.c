#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int fd_parent[2], fd_child[2];
    int result;
    size_t size;
    char resstring[14];

    if (pipe(fd_parent) < 0 || pipe(fd_child) < 0) {
       printf("Can\'t open pipe.\n");
       exit(-1);
    }

    result = fork();
    if (result < 0) {
       printf("Can\'t fork child.\n");
       exit(-1);
    } else if (result > 0) {
       close(fd_parent[0]);
       close(fd_child[1]);
       size = write(fd_parent[1], "Hello, world!", 14);
       if (size != 14) {
          printf("Can\'t write whole string to pipe.\n");
          exit(-1);
       }

       close(fd_parent[1]);

       size = read(fd_child[0], resstring, 14);
       if (size != 14) {
          printf("Can\'t read whole string from pipe child.\n");
          exit(-1);
       }

       printf("Result string from child: %s\n", resstring);
       printf("Exit parent.\n");
    } else {
       close(fd_parent[1]);
       close(fd_child[0]);

       size = read(fd_parent[0], resstring, 14);
       if (size < 0) {
          printf("Can\'t read whole string from pipe parent.\n");
          exit(-1);
       }

       printf("Exit child.\nResult: %s\n", resstring);

       for (int i = 0; i < 13 / 2; ++i) {
           int temp = resstring[i];
           resstring[i] = resstring[13 - i - 1];
           resstring[13 - i - 1] = temp;
       }

       size = write(fd_child[1], resstring, 14);
       close(fd_parent[0]);
     }

     return 0;
}
