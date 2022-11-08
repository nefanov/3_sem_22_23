#ifndef _HEAD_H_
#define _HEAD_H_


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/msg.h>


#define BF_SIZE 4000

#define LAST_MSG 2

typedef struct buf
{
	long mtype;
	char mtext[BF_SIZE];
	
} buf;


size_t send(int msqid);

size_t read(int msqid);

int get_msqid(const char *name);

#endif // _HEAD_H_