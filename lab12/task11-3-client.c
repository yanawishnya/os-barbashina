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
    double input;

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

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get msqid\n");
        exit(-1);
    }

    printf("Type double number ");
    scanf("%lf", &input);

    mybuf.mtype = 1;
    mybuf.info.pid = getpid();
    mybuf.info.finfo = input;
    len = sizeof(mybuf.info);

    if (msgsnd(msqid, (struct mymsgbuf *) &mybuf, len, 0) < 0) {
        printf("Can't send message to queue\n");
        exit(-1);
    }

    printf("Client sent message to server\n");
    printf("Client starts reading message\n");
    maxlen = sizeof(mybuf.info);

    if ((len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, getpid(), 0)) < 0) {
        printf("Can't receive message from queue\n");
        exit(-1);
    }

    printf("Client received: message type = %ld, server pid = %d, fInfo = %f\n", mybuf.mtype, mybuf.info.pid,
           mybuf.info.finfo);
    printf("Client finished\n");
    return 0;
}
