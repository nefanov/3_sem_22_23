#include "shm.h"




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
		if ((shmid = shmget(key, BF_SIZE * sizeof(char), 0666|IPC_CREAT|IPC_EXCL)) < 0)
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
		if ((shmid = shmget(key, sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0)
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


void del_shmem(char *shmem)
{
	if (shmdt(shmem) < 0)
	{
		perror("shmdt");
		exit(-1);
	}
}


size_t send(char *shmem, int *shnum)
{
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

void init_sems(int semid, struct sembuf *mybuf0)
{
	if (semop(semid, mybuf0, 2) < 0)
	{
		perror("semop");
		exit(-1);
	}
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

