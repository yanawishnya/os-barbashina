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
    char *text;
    int fsize = 0;
    int *fsize_ptr;

    if ((key = ftok(fname, 0)) < 0) {
       printf("Can\'t generate key.\n");
       exit(-1);
    }

    if ((shmid = shmget(key, sizeof(int) + fsize * sizeof(char), 0)) < 0) {
       printf("Can\'t create shared memory.\n");
       exit(-1);
    }

    if ((fsize_ptr = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
       printf("Can\'t attach shared memory.\n");
       exit(-1);
    }

    fsize = *fsize_ptr;
    text = (char *)(fsize_ptr + 1);

    for (int i = 0; i < fsize; ++i) {
        putchar(text[i]);
    }

    if (shmdt(fsize_ptr) < 0) {
       printf("Can\'t detach shared memory.\n");
       exit(-1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
       printf("Can\'t remove shared memory.\n");
       exit(-1);
    }

    printf("Reading has completed successfully.\n");
    return 0;
}
