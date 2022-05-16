#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // IPC descriptor for the message queue
    int msqid;
    // The file name used to generate the key.
    char pathname[] = "task11-3-server.c";
    // IPC key
    key_t key;
    // Length of the informative part of the message
    int len, maxlen;

    struct mymsgbuf {
        long mtype;
        struct {
            pid_t pid;
            double finfo;
        } info;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666)) < 0) {
        printf("Can't get message queue\n");
        exit(-1);
    }

    mybuf.mtype = 2;
    mybuf.info.pid = getpid();
    len = sizeof(mybuf.info);

    if (msgsnd(msqid, (struct mymsgbuf *) &mybuf, len, 0) < 0) {
        printf("Can't send message to queue\n");
        exit(-1);
    }

    printf("Killer sent kill message to server\n");
    printf("Killer finished\n");

    return 0;
}
