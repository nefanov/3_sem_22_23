#ifndef _SHM_H_
#define _SHM_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../CommonFunctions/CommonFunctions.hpp"

const char FILENAME[] = "/mnt/c/Users/olegb/YandexDisk-berezin.op@phystech.edu/Учёба/CT/3_sem_22_23/task3/solution/SharedMemory/Makefile";

enum CONSTANTS {
    SMALL = 8,
    MEDIUM = 1024,
    BIG = 32768
};

#endif