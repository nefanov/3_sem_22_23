#ifndef _MSQ_H_
#define _MSQ_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERTED(func, var, cntrlValue, retValue)   \
    if (strcmp(#func, "NP") == 0) {                 \
        if (var == cntrlValue) {                    \
            perror("null pointer passed");          \
                                                    \
            return retValue;                        \
        }                                           \
    } else if (var == cntrlValue) {                 \
        perror(#func"() failed");                   \
        return retValue;                            \
    }

const char FILENAME[] = "/mnt/c/Users/olegb/YandexDisk-berezin.op@phystech.edu/Учёба/CT/3_sem_22_23/task3/solution/MessageQueues/Makefile";

enum CONSTANTS {
    SMALL = 8,
    MEDIUM = 1024,
    BIG = 4096
};


#endif