#include "FIFO.h"

int main()
{
	int fifo;
	size_t letter = 1;
	char name[] = "file.fifo";


	(void)umask(0);


	int shnumid = get_shmid("main1.c");
	int *shnum = get_shnum(shnumid);//разделяемая память для количества записанных байт
	
	struct sembuf *wait, *after_send;
	
	wait = (struct sembuf*)malloc(2 * sizeof(struct sembuf));
	after_send = (struct sembuf*)malloc(2 * sizeof(struct sembuf));
	
	get_sembuf_for_send(wait, after_send);

	int semid = get_semid("main1.c");

	init_sems(semid);


	if (mknod(name, S_IFIFO | 0666, 0)  < 0)
	{
		perror("mknod");
		exit(-1);
	}

	if ((fifo = open(name, O_WRONLY)) < 0)
	{
		perror("open");
		exit(-1);
	}
	

	while (letter != 0)
	{
		do_it(semid, wait);

		letter = send(name, shnum, fifo);

		do_it(semid, after_send);
	}


	close(fifo);

	return 0;

}