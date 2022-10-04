#include "shm.h"


//0 - mutax 1 - empty 2 -full

int main()
{
	int letter = 1;

	int shmid = get_shmid("main2.c", 0);

	int shnumid = get_shmid("main1.c", 1);

	char *shmem = get_shmem(shmid);

	int *shnum = get_shnum(shnumid);

	struct sembuf *mybuf, *mybuf1, *mybuf0;
	//get_sembuf_ops(mybuf0, mybuf, mybuf1);
	mybuf = (struct sembuf*)malloc(2 * sizeof(struct sembuf));
	mybuf1 = (struct sembuf*)malloc(2 * sizeof(struct sembuf));
	mybuf0 = (struct sembuf*)malloc(2 * sizeof(struct sembuf));

	mybuf0[0].sem_op = 1;
	mybuf0[0].sem_flg = 0;
	mybuf0[0].sem_num = 0;

	mybuf0[1].sem_op = 1;
	mybuf0[1].sem_flg = 0;
	mybuf0[1].sem_num = 1;

	mybuf[0].sem_op = -1;
	mybuf[0].sem_flg = 0;
	mybuf[0].sem_num = 1;

	mybuf[1].sem_op = -1;
	mybuf[1].sem_flg = 0;
	mybuf[1].sem_num = 0;

	mybuf1[0].sem_op = 1;
	mybuf1[0].sem_flg = 0;
	mybuf1[0].sem_num = 0;

	mybuf1[1].sem_op = 1;
	mybuf1[1].sem_flg = 0;
	mybuf1[1].sem_num = 2;


	int semid = get_semid("main1.c", 2);
	init_sems(semid, mybuf0);

	while (letter != 0)
	{
		if (semop(semid, mybuf, 2) < 0)
		{
			perror("semop");
			exit(-1);
		}

		//printf("sending...\n");
		letter = send(shmem, shnum);
		//printf("waiting...\n");

		if (semop(semid, mybuf1, 2) < 0)
		{
			perror("semop");
			exit(-1);
		}
	}

	return 0;
}


