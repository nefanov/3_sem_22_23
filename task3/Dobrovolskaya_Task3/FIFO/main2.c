#include "FIFO.h"

int main()
{
	int fifo;
	size_t letter = 1;
	char name[] = "file.fifo";


	(void)umask(0);


	int shnumid = get_shmid("main1.c");
	int *shnum = get_shnum(shnumid);//разделяемая память для количества записанных байт

	struct sembuf *wait, *after_read;
	
	wait = (struct sembuf*)malloc(2 * sizeof(struct sembuf));
	after_read = (struct sembuf*)malloc(2 * sizeof(struct sembuf));
	
	get_sembuf_for_read(wait, after_read);

	int semid = get_semid("main1.c");


	mknod(name, S_IFIFO | 0666, 0);

	if ((fifo = open(name, O_RDONLY)) < 0)
	{
		perror("open");
		exit(-1);
	}


	while (letter != 0)
	{
		do_it(semid, wait);

		letter = read(name, shnum, fifo);

		do_it(semid, after_read);
	}


	del_fifo(name);
	del_sem(semid);
	del_shm(shnum);


	return 0;

}
