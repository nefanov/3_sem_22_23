#include "shm.h"


//0 - mutex 1 - empty 2 -full

int main()
{
	int letter = 1;
	int semid;
	
	key_t key;
	struct sembuf *mybuf, *mybuf1;
	int start = 0;
	mybuf = (struct sembuf*)malloc(2 * sizeof(struct sembuf));
	mybuf1 = (struct sembuf*)malloc(2 * sizeof(struct sembuf));

//---------------------------------------------
	int shmid = get_shmid("main2.c", 0);

	int shnumid = get_shmid("main1.c", 1);

	char *shmem = get_shmem(shmid);

	int *shnum = get_shnum(shnumid);
//----------------------------------------------
	
	if ((key = ftok("main1.c", 2)) < 0)
	{
		perror("ftok");
		exit(-1);
	}

	if ((semid = semget(key, 3, 0666 | IPC_CREAT)) < 0)
	{
		perror("semget");
		exit(-1);
	}

	mybuf[0].sem_op = -1;
	mybuf[0].sem_flg = 0;
	mybuf[0].sem_num = 2;

	mybuf[1].sem_op = -1;
	mybuf[1].sem_flg = 0;
	mybuf[1].sem_num = 0;

	mybuf1[0].sem_op = 1;
	mybuf1[0].sem_flg = 0;
	mybuf1[0].sem_num = 0;

	mybuf1[1].sem_op = 1;
	mybuf1[1].sem_flg = 0;
	mybuf1[1].sem_num = 1;
	

	while (letter != 0)
	{
		if (semop(semid, mybuf, 2) < 0)
		{
			perror("semop");
			exit(-1);
		}
		//printf("reading...\n");
		letter = read(shmem, shnum);
		//printf("waiting...\n");

		if (semop(semid, mybuf1, 2) < 0)
		{
			perror("semop");
			exit(-1);
		}
	}

	del_sem(semid);
	del_shm(shmem, shnum);

	return 0;
}



  