#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <semaphore.h>

#define KEY_FILE "ma_key"
#define BUFFER_SIZE 1024

#define INPUT_NAME "input.txt"
#define OUTPUT_NAME "output.txt"
#define READ_SEM_NAME "/read_signal"
#define WRITE_SEM_NAME "/write_signal"

#define MAX_CMD_LEN 100
#define PERMISSIONS 0666

void logr (const char* format, ...);
void logs (const char* format, ...);

struct messageBuf
{
    long mtype;
    char mtext[BUFFER_SIZE];
};