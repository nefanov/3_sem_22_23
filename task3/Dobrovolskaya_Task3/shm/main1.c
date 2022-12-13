#include "shm.h"




int main()
{
	int letter = 1;

	int shmid = get_shmid("main2.c", 0);

	int shnumid = get_shmid("main1.c", 1);

	char *shmem = get_shmem(shmid);//разделяемая память для записи 

	int *shnum = get_shnum(shnumid);//разделяемая память для количества записанных байт

	struct sembuf *wait, *after_send;
	
	wait = (struct sembuf*)malloc(2 * sizeof(struct sembuf));
	after_send = (struct sembuf*)malloc(2 * sizeof(struct sembuf));
	
	get_sembuf_for_send(wait, after_send);

	int semid = get_semid("main1.c", 2);
	init_sems(semid);

	while (letter != 0)
	{
		do_it(semid, wait);

		letter = send(shmem, shnum);

		do_it(semid, after_send);
	}

	return 0;
}


