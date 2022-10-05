#include "shm.h"




int main()
{
	int letter = 1;

	int shmid = get_shmid("main2.c", 0);

	int shnumid = get_shmid("main1.c", 1);

	char *shmem = get_shmem(shmid);//разделяемая память для записи 

	int *shnum = get_shnum(shnumid);//разделяемая память для количества записанных байт

	struct sembuf *wait, *after_read;
	
	wait = (struct sembuf*)malloc(2 * sizeof(struct sembuf));
	after_read = (struct sembuf*)malloc(2 * sizeof(struct sembuf));
	
	get_sembuf_for_read(wait, after_read);

	int semid = get_semid("main1.c", 2);

	while (letter != 0)
	{
		do_it(semid, wait);

		letter = read(shmem, shnum);

		do_it(semid, after_read);
	}

	del_sem(semid);
	del_shm(shmem, shnum);

	return 0;
}



  