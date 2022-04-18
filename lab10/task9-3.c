#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

// Parent:
//
// while {
// write 
// add
// wait
// read
// }

// Child:
//
// while {
// decrease
// read 
// write
// decrease
// }

int decrease(int semid, struct sembuf *buf){
  buf->sem_op = -1;
  buf->sem_flg = 0;
  buf->sem_num = 0;
  return semop(semid, buf, 1);
}

int add(int semid, struct sembuf *buf){
  buf->sem_op = 2;
  buf->sem_flg = 0;
  buf->sem_num = 0;
  return semop(semid, buf, 1);
}

int wait(int semid, struct sembuf *buf){
  buf->sem_op = 0;
  buf->sem_flg = 0;
  buf->sem_num = 0;
  return semop(semid, buf, 1);
}

int main() {
  int fd[2], result, semid;
  size_t size;
  char resstring[14];
  key_t key;
  struct sembuf mybuf;

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe.\n");
    exit(-1);
  }

  semctl(semid, 0, IPC_RMID, 0);

  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t creat semaphore.\n");
    exit(-1);
  }

  int num;
  scanf("%d", &num);
  int count = 0;
  result = fork();

  if (result < 0) {
    printf("Can\'t fork child.\n");
    exit(-1);
  } else if (result > 0) {
    while(count < num) {
       size = write(fd[1], "Hello, child!", 14);
       if (size != 14) {
         printf("Can\'t write all string to pipe.\n");
         exit(-1);
       }
       count++;
       printf("Parent sent message.\n");
       add(semid, &mybuf);
       wait(semid, &mybuf);
       size = read(fd[0], resstring, 15);
       if (size < 0) {
         printf("Can\'t read string.\n");
         exit(-1);
       }

       printf("Message to parent: %s.\n", resstring);
     }
  } else {
    while(count < num) {
       decrease(semid, &mybuf);
       size = read(fd[0], resstring, 14);
       if (size < 0) {
         printf("Can\'t read string from pipe.\n");
         exit(-1);
       }
       printf("Message to child: %s.\n", resstring);
       size = write(fd[1], "Hello,parent!", 14);
       if (size < 0) {
         printf("Can\'t write string to pipe\n");
         exit(-1);
       }
       count++;
       printf("Child sent message.\n");
       decrease(semid, &mybuf);
    }
  }
  return 0;
}
