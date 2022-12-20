#include "head.hpp"


int get_shmid(const char *name, int num)
{
	int shmid;
	key_t key;

	if ((key = ftok(name, num)) < 0)
	{
		perror("ftok");
		exit(-1);
	}

	if (num == 0)
	{
		if ((shmid = shmget(key, BF_SIZE * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
		{
			if (errno != EEXIST)
			{
				perror("shmget");
				exit(-1);
			}
			else
			{
				if ((shmid = shmget(key, BF_SIZE * sizeof(char), 0)) < 0)
				{
					perror("shmget");
					exit(-1);
				}
			}
		}

	}
	else
	{
		if ((shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
		{
			if (errno != EEXIST)
			{
				perror("shmget");
				exit(-1);
			}
			else
			{
				if ((shmid = shmget(key, sizeof(int), 0)) < 0)
				{
					perror("shmget");
					exit(-1);
				}
			}
	}

	}

	return shmid;

}


int get_semid(const char *name, int num)
{
	key_t key;
	int semid;

	if ((key = ftok(name, num)) < 0)
	{
		perror("ftok");
		exit(-1);
	}

	if ((semid = semget(key, 3, 0666 | IPC_CREAT)) < 0)
	{
		perror("semget");
		exit(-1);
	}

	return semid;
}


char *get_shmem(int shmid)
{
	char *shmem;

	if ((shmem = (char*)shmat(shmid, NULL, 0)) == (char*)(-1))
	{
		perror("shmat");
		exit(-1);
	}

	return shmem;
}


int *get_shnum(int shmid)
{
	int *shnum;

	if ((shnum = (int*)shmat(shmid, NULL, 0)) == (int*)(-1))
	{
		perror("shmat");
		exit(-1);
	}

	return shnum;
}


size_t send(char *shmem, int *shnum)
{
	assert(shmem);

	size_t size;
	static int fd = 0;

    if (fd == 0)
    {
        if ((fd = open("file.txt", O_RDONLY)) < 0)
        {
            perror("open");
            _exit(-1);
        }
    }
    
    size = read(fd, shmem, BF_SIZE);

    *shnum = size;

	if (size < 0)
    {
        close(fd);
        perror("read");
        _exit(-1);
    }
    
    return size;

}


size_t read(char *shmem, int *shnum)
{
	assert(shmem);

	size_t size;
	int count;
	static int fd = 0;

	if (fd == 0)
    {
        if ((fd = open("text.txt", O_WRONLY | O_CREAT, 0666)) < 0)
        {
            perror("open");
            _exit(-1);
        }
    }

    count = *shnum;

    size = write(fd, shmem, count);
    
    if (size < 0)
    {
        close(fd);
        perror("write");
        _exit(-1);
    } 

    return size; 

}


void init_sems(int semid)
{
	struct sembuf *sems;
	sems = (struct sembuf*)malloc(2 * sizeof(struct sembuf));

	sems[0].sem_op = 1;
	sems[0].sem_flg = 0;
	sems[0].sem_num = 0;

	sems[1].sem_op = 1;
	sems[1].sem_flg = 0;
	sems[1].sem_num = 1;

	if (semop(semid, sems, 2) < 0)
	{
		perror("semop");
		exit(-1);
	}

	free(sems);
}


void del_sem(int semid)
{
	if (semctl(semid, 0, IPC_RMID, 0) < 0)
	{
		perror("semctl");
		exit(-1);
	}
}


void del_shm(char *shmem, int *shnum)
{
	assert(shmem);
	assert(shnum);

	if (shmdt(shmem) < 0)
	{
		perror("shmdt");
		exit(-1);
	}

	if (shmdt(shnum) < 0)
	{
		perror("shmdt");
		exit(-1);
	}
}


void get_sembuf_for_send(struct sembuf *wait, struct sembuf *after_send)
{
	assert(wait);
	assert(after_send);

	wait[0].sem_op = -1;
	wait[0].sem_flg = 0;
	wait[0].sem_num = 1;

	wait[1].sem_op = -1;
	wait[1].sem_flg = 0;
	wait[1].sem_num = 0;

	after_send[0].sem_op = 1;
	after_send[0].sem_flg = 0;
	after_send[0].sem_num = 0;

	after_send[1].sem_op = 1;
	after_send[1].sem_flg = 0;
	after_send[1].sem_num = 2;
}


void get_sembuf_for_read(struct sembuf *wait, struct sembuf *after_read)
{
	assert(wait);
	assert(after_read);
	
	wait[0].sem_op = -1;
	wait[0].sem_flg = 0;
	wait[0].sem_num = 2;

	wait[1].sem_op = -1;
	wait[1].sem_flg = 0;
	wait[1].sem_num = 0;

	after_read[0].sem_op = 1;
	after_read[0].sem_flg = 0;
	after_read[0].sem_num = 0;

	after_read[1].sem_op = 1;
	after_read[1].sem_flg = 0;
	after_read[1].sem_num = 1;
}


void do_it(int semid, struct sembuf *sembuf)
{
	assert(sembuf);
	
	if (semop(semid, sembuf, 2) < 0)
	{
		perror("semop");
		exit(-1);
	}
}
	