#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/sem.h>

void P(int semid) {
  struct sembuf buf;
  buf.sem_num = 0;
  buf.sem_op = -1;
  buf.sem_flg = 0;

  if (semop(semid, &buf, 1) < 0) {
     printf("Can\'t wait for condition.\n");
     exit(-1);
  }
}

void V(int semid) {
  struct sembuf buf;
  buf.sem_num = 0;
  buf.sem_op = 1;
  buf.sem_flg = 0;

  if (semop(semid, &buf, 1) < 0) {
     printf("Can\'t wait for condition.\n");
     exit(-1);
  }
}

int main() {
  int     *array;
  int     shmid;
  int     new = 1;
  char    pathname[] = "task7-3a.c";
  key_t   key;
  long    i;
  int     semid;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key.\n");
    exit(-1);
  }

  if ((shmid = shmget(key, 3 * sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory.\n");
      exit(-1);
    } else {
      if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0) {
        printf("Can\'t find shared memory.\n");
        exit(-1);
      }
      new = 0;
    }
  }

  if ((semid = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if ((semid = semget(key, 1, 0)) < 0) {
      printf("Can\'t create shared memory.\n");
      exit(-1);
    }
  } else {
      V(semid);
  }

  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory.\n");
    exit(-1);
  }

  P(semid);
  if (new) {
    array[0] =  0;
    array[1] =  1;
    array[2] =  1;
  } else {
    array[1] += 1;
    for(i = 0; i < 2000000000L; i++);
    array[2] += 1;
  }

  printf
    ("Program 1 was spawn %d times, program 2 - %d times, total - %d times.\n",
    array[0], array[1], array[2]);
  V(semid);

  if (shmdt(array) < 0) {
    printf("Can't detach shared memory.\n");
    exit(-1);
  }

  return 0;
}
