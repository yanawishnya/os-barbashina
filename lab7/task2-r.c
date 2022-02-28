#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

int main() {
    int shmid;
    char fname[] = "task1-w.c";
    key_t key;
    char *code;
    int fsize = 0;
    int *fsize_ptr;
    FILE *file;
    file = fopen(fname, "r");

    fseek(file, 0L, SEEK_END);
    fsize = ftell(file);
    rewind(file);

    if ((key = ftok(fname, 0)) < 0) {
       printf("Can\'t generate key.\n");
       exit(-1);
    }

    if ((shmid = shmget(key, sizeof(int) + fsize * sizeof(char), 
                0666 | IPC_CREAT)) < 0) {
       printf("Can\'t create shared memory.");
       exit(-1);
    }

    if ((fsize_ptr = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
       printf("Can\'t attach shared memory.\n");
       exit(-1);
    }

    *fsize_ptr = fsize;
    code = (char *)(fsize_ptr + 1);

    for (int i = 0; i < fsize; ++i) {
        code[i] = fgetc(file);
    }

    fclose(file);

    if (shmdt(fsize_ptr) < 0) {
       printf("Can\'t detach shared memory.\n");
       exit(-1);
    }

    printf("Writing has completed successfully.\n");
    return 0;
}
