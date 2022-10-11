#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


#define BF_SIZE 4080


size_t send(const char *name, int *shnum, int fifo);
size_t read(const char *name, int *shnum, int fifo);
char *get_shmem(int shmid);
int *get_shnum(int shmid);
int get_shmid(const char *name);
int get_semid(const char *name);
void del_shm(int *shnum);
void del_sem(int semid);
void get_sembuf_for_send(struct sembuf *wait, struct sembuf *after_send);
void get_sembuf_for_read(struct sembuf *wait, struct sembuf *after_read);
void init_sems(int semid);//0 - mutax 1 -empty 2 - full
void do_it(int semid, struct sembuf *sembuf);
void del_fifo(const char *name);
int open_fifo(const char *name);



