#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


#define BF_SIZE 20000


size_t send(char *shmem, int *shnum);
size_t read(char *shmem, int *shnum);
char *get_shmem(int shmid);
int *get_shnum(int shmid);
int get_shmid(const char *name, int num);
void del_shmem(char *shmem);
void del_shm(char *shmem, int *shnum);
void del_sem(int semid);
//void get_sembuf_ops(struct sembuf *mybuf0, struct sembuf *mybuf, struct sembuf *mybuf1);
void init_sems(int semid, struct sembuf *mybuf0);
int get_semid(const char *name, int num);



