#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

int main() {
    int fd[2];
    size_t size = 1;
    int res = 0;
    char* resstring = malloc(sizeof(char));

    if (pipe(fd) < 0) {
       printf("Can\'t creat pipe.\n");
       exit(-1);
    }

    fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);

    while (size == 1) {
       size = write(fd[1], resstring, 1);
       if (size != 1) {
		break;
       }
       res++;
    }

    printf("RESULT: \n\tsize = %d bytes\n", res);
    close(fd[0]);
    close(fd[1]);
    free(resstring);
    return 0;
}
