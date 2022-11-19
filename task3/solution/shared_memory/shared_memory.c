#include "shared_memory.h"
const char* key_name = "./times";
const char* key_data_name = "./data.txt";

void* get_mem(const char* path, size_t buf_size, int* shm_d){
  key_t key = ftok(path, 1);
  if (key == -1) {
    perror("ftok error for");
    exit(1);
  }

  *shm_d = shmget(key, buf_size, IPC_CREAT | 0666);
  if (*shm_d == -1) {
    perror(" shmget error");
    exit(1);
  }

  void* shmptr = shmat(*shm_d, NULL, 0666);
  if (shmptr == NULL) {
    perror("shmat error");
    exit(1);
  }

  return shmptr;
}

void init_sems(int sem_d, int semval_1, int semval_2, int semval_3) {
  if (sem_d < 0) {
    perror("[constructor]semaphore create");
    exit(1);
  }
  if (semctl(sem_d, 0, SETVAL, semval_1) < 0 || semctl(sem_d, 1, SETVAL, semval_2) < 0 || semctl(sem_d, 2, SETVAL, semval_3)) {
    perror("[constructor]constructing semaphore");
    exit(1);
  }
}

void lock_sem(int sem_d, sem_name semnum) {
  struct sembuf operation;
  operation.sem_op = -1;
  operation.sem_num = semnum;
  operation.sem_flg = 0;
  if(semop(sem_d, &operation, 1) < 0) {
    if (semnum == MUTEX){
      perror("[lock]mutex semaphore lock");
    } else if (semnum == WRITE) {
      perror("[lock]write semaphore lock");
    } else if (semnum == READ) {
      perror("[lock]read semaphore lock");
    } else {
      perror("[lock]semaphore lock");
    }
  }
}

void unlock_sem(int sem_d, sem_name semnum) {
  struct sembuf operation;
  operation.sem_op = 1;
  operation.sem_num = semnum;
  operation.sem_flg = 0;
  if(semop(sem_d, &operation, 1) < 0) {
    if (semnum == MUTEX){
      perror("[unlock]mutex semaphore lock");
    } else if (semnum == WRITE) {
      perror("[unlock]write semaphore lock");
    } else if (semnum == READ) {
      perror("[unlock]read semaphore lock");
    } else {
      perror("[unlock]semaphore lock");
    }
  }
}

int shared_mem_snd(const char* fname) {
  int res = 0, nwrite  = 1;
  int buff_d = 0, snt_d = 0;
  key_t key = ftok(key_name, 2);

  
  char* buffer = (char*) get_mem("./wrapper.py", BUF_SIZE, &buff_d);
  double* sent = (double*) get_mem("./Makefile", sizeof(double), &snt_d);
  
  // Making semaphore array
  // semaphores = {write_lock, read_lock, mutex}
  // write_lock stops rcv process when it wants to read to file non ready data in buffer
  int sem_d = semget(key, 3, IPC_CREAT | 0666);
  init_sems(sem_d, 1, 0, 1);

  FILE* file_d = fopen(fname, "r");
  assert(file_d != NULL);

  while (nwrite != 0) {
    lock_sem(sem_d, WRITE);
    lock_sem(sem_d, MUTEX);
    nwrite = fread(buffer, sizeof(char), BUF_SIZE, file_d);
    res += nwrite;
    *sent = nwrite;
    unlock_sem(sem_d, MUTEX);
    unlock_sem(sem_d, READ);
  }
  lock_sem(sem_d, 0);
  fclose(file_d);
  shmctl(buff_d, IPC_RMID, NULL);
  shmctl(snt_d, IPC_RMID, NULL);
  shmdt(buffer); shmdt(sent);
  
  return res;
}

int shared_mem_rcv(const char* fname) {
  int res = 0, nread  = 1;
  int buff_d = 0, snt_d = 0;
  key_t key = ftok(key_name, 2);

  char* buffer = (char*) get_mem("./wrapper.py", BUF_SIZE, &buff_d);
  double* sent = (double*) get_mem("./Makefile", (size_t) sizeof(double), &snt_d);
  
  // Making semaphore array
  // semaphores = {write_lock, read_lock, mutex}
  // write_lock stops rcv process when it wants to read to file non ready data in buffer
  int sem_d = semget(key, 3, 0666);

  FILE* file_d = fopen(fname, "w");
  assert(file_d != NULL);

  while (nread != 0) {
    lock_sem(sem_d, READ);
    lock_sem(sem_d, MUTEX);
    nread = fwrite(buffer, sizeof(char), *sent,  file_d);
    res += nread;
    unlock_sem(sem_d, MUTEX);
    unlock_sem(sem_d, WRITE);
    
  }
  fclose(file_d);
  //semctl(sem_d, 0, IPC_RMID, 0);
  shmdt(buffer); shmdt(sent);
  
  return res;
}