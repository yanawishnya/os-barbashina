#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

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

    if ((msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno == EEXIST) {
            printf("Only one instance of server is allowed\n");
            exit(0);
        } else {
            printf("Can't get msqid\n");
            exit(-1);
        }
    }

    printf("Server starts reading messages\n");

    while (1) {
        maxlen = sizeof(mybuf.info);

        if ((len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, -2, 0)) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }

        if (mybuf.mtype == 2) {
            printf("Server received kill instruction\n");
            break;
        }

        printf("Server received: message type = %ld, client pid = %d, fInfo = %f\n", mybuf.mtype, mybuf.info.pid,
               mybuf.info.finfo);
        printf("Server sent a response\n");

        mybuf.mtype = mybuf.info.pid;
        mybuf.info.pid = getpid();
        mybuf.info.finfo *= mybuf.info.finfo;
        len = sizeof(mybuf.info);

        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
            printf("Can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }

    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    printf("Server finished\n");

    return 0;
}
