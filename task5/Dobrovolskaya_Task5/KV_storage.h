#ifndef KV_STORAGE
#define KV_STORAGE

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <pthread.h>

#define N_PAGE 100
#define PAGE_SIZE 8 * 1024

char *STORAGE;

FILE *LOG;

pid_t CHILD_PID, PARENT_PID;

int get_shmid();
void parent_handler(int signo, siginfo_t *si, void *ucontext);
void child_handler(int signo, siginfo_t *si, void *ucontext);
void sigint_handler(int signo);

pthread_mutex_t mutex;



void parent_handler(int signo, siginfo_t *si, void *ucontext)
{
	int n_page = 0;
	int offset = 0;
	int len;

	if ((len = read(0, STORAGE, PAGE_SIZE * N_PAGE)) == -1)
	{
		perror("read");
		_exit(1);
	}

	STORAGE[len - 1] = '\0';

	pthread_mutex_lock(&mutex);
	
	LOG = fopen("log.txt", "a");
	fprintf(LOG, "В хранилище была записана строка: %s\n", STORAGE);
	fclose(LOG);

	pthread_mutex_unlock(&mutex);

	union sigval value;
	value.sival_int = n_page * 10000 + offset;

	if (sigqueue(CHILD_PID, SIGUSR1, value) == -1)
	{
		perror("sigqueue");
		_exit(1);
	}

}


void child_handler(int signo, siginfo_t *si, void *ucontext)
{
	int value_int = (si->si_value).sival_int;
	int n_page = value_int / 10000;
	int offset = value_int % 10000;

	pthread_mutex_lock(&mutex);

	LOG = fopen("log.txt", "a");
	fprintf(LOG, "Из хранилища была считана строка: %s\n", STORAGE);
	fclose(LOG);

	pthread_mutex_unlock(&mutex);

	printf("Считанная из хранилища строка: %s\n", STORAGE + PAGE_SIZE * n_page + offset);

	union sigval value;
	value.sival_ptr = (void*)"ACKnowledgement";

	if (sigqueue(PARENT_PID, SIGUSR2, value) == -1)
	{
		perror("sigqueue");
		_exit(1);
	}
}


void sigint_handler(int signo)
{
	if (shmdt(STORAGE) < 0)
	{
		perror("shmdt");
		_exit(-1);
	}

	_exit(1);
}


int get_shmid()
{
	key_t key;
	int shmid;

	if ((key = ftok("KV_storage.c", 0)) < 0)
	{
		perror("ftok");
		_exit(-1);
	}

	if ((shmid = shmget(key, PAGE_SIZE * N_PAGE * sizeof(char), 0666|IPC_CREAT|IPC_EXCL)) < 0)
	{
		if (errno != EEXIST)
		{
			perror("shmget");
			_exit(-1);
		}
		else
		{
			if ((shmid = shmget(key, PAGE_SIZE * N_PAGE * sizeof(char), 0)) < 0)
			{
				perror("shmget");
				exit(-1);
			}
		}
	}

	return shmid;
}



#endif
