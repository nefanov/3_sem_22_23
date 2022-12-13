#include "FIFO.h"




int get_shmid(const char *name)
{
	int shmid;
	key_t key;

	if ((key = ftok(name, 0)) < 0)
	{
		perror("ftok");
		exit(-1);
	}
	
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

	return shmid;

}


int get_semid(const char *name)
{
	key_t key;
	int semid;

	if ((key = ftok(name, 1)) < 0)
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


size_t send(const char *name, int *shnum, int fifo)
{
	assert(shnum);

	size_t size;
	int get;
	static int file = 0;
	
	char mem[BF_SIZE];

    if (file == 0)
    {
        if ((file = open("file.txt", O_RDONLY)) < 0)
        {
            perror("open");
            _exit(-1);
        }
    }

    get = read(file, mem, BF_SIZE);

	if (get < 0)
	{
    	close(file);
    	close(fifo);
		perror("read");
		exit(-1);
	}

    *shnum = get;
    

	size = write(fifo, mem, get);
	

	if (size < 0)
	{
    	close(file);
    	close(fifo);
		perror("write");
		exit(-1);
	}
	else if (size == 0)
	{
		close(file);
		close(fifo);
	}

    return size;

}


size_t read(const char *name, int *shnum, int fifo)
{
	assert(shnum);

	size_t size;
	int count;
	static int file = 0;
	
	char mem[BF_SIZE];

	if (file == 0)
    {
        if ((file = open("text.txt", O_WRONLY | O_CREAT, 0666)) < 0)
        {
            perror("open");
            _exit(-1);
        }

    }

    count = *shnum;
    

    size = read(fifo, mem, count);

    if (size < 0)
    {
    	close(file);
    	close(fifo);
        perror("read");
        _exit(-1);
    } 

    size = write(file, mem, size);
    
    if (size < 0)
    {
    	close(file);
    	close(fifo);
        perror("write");
        _exit(-1);
    }
    else if (size == 0)
    {
    	close(file);
    	close(fifo);
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


void del_shm(int *shnum)
{
	assert(shnum);

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


void del_fifo(const char *name)
{
	if (remove("file.fifo") < 0)
	{
		perror("remove");
		exit(-1);
	}
}


int open_fifo(const char *name)
{
	int fifo;
	
	mknod(name, S_IFIFO | 0666, 0);

	if ((fifo = open(name, O_RDONLY)) < 0)
	{
		perror("open");
		exit(-1);
	}

	return fifo;
}
