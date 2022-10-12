#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


#define BF_SIZE 4000


size_t send(char *shmem, int *shnum);
size_t read(char *shmem, int *shnum);
char *get_shmem(int shmid);
int *get_shnum(int shmid);
int get_shmid(const char *name, int num);
void del_shm(char *shmem, int *shnum);
void del_sem(int semid);
void get_sembuf_for_send(struct sembuf *wait, struct sembuf *after_send);
void get_sembuf_for_read(struct sembuf *wait, struct sembuf *after_read);
void init_sems(int semid);//0 - mutax 1 - empty 2 -full
int get_semid(const char *name, int num);
void do_it(int semid, struct sembuf *sembuf);



