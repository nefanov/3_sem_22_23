#include "msq.h"




int get_msqid(const char *name)
{
	int msqid;
	key_t key;

	if ((key = ftok(name, 0)) < 0)
	{
		perror("ftok");
		exit(-1);
	}


	if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
	{
		perror("msqget");
		exit(-1);
	}

	return msqid;

}


size_t send(int msqid)
{
	int get;
	static int file = 0;
	buf buffer;
	char string[BF_SIZE];
	

    if (file == 0)
    {
        if ((file = open("file.txt", O_RDONLY)) < 0)
        {
            perror("open");
            _exit(-1);
        }
    }

	buffer.mtype = 1;

    get = read(file, buffer.mtext, BF_SIZE);

	if (get < 0)
	{
    	close(file);
		perror("read");
		exit(-1);
	}
	else if (get == 0)
	{
		close(file);
	}
	else if (get < BF_SIZE)
	{
		buffer.mtype = LAST_MSG;
	}

	if (msgsnd(msqid, (struct msgbuf *) &buffer, get, 0) < 0)
	{
		perror("msqsnd");
		msgctl(msqid, IPC_RMID, NULL);
		exit(-1);
	}

    return get;

}


size_t read(int msqid)
{
	size_t size;
	int len;
	static int file = 0;
	buf buffer;
	
	if (file == 0)
    {
        if ((file = open("text.txt", O_WRONLY | O_CREAT, 0666)) < 0)
        {
            perror("open");
            _exit(-1);
        }

    }

	if ((len = msgrcv(msqid, (struct msgbuf *) &buffer, BF_SIZE, 0, 0)) < 0)
	{
		perror("msqrcv");
		exit(-1);
	}
	
	size = write(file, buffer.mtext, len);

	if (size < 0)
	{
		perror("write");
		exit(-1);
	}

	if (buffer.mtype == LAST_MSG)
	{
		close(file);
		msgctl(msqid, IPC_RMID, NULL);
		exit(0);
	}

    return size; 

}

