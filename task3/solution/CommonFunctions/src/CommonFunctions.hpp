#ifndef _COMMON_FUNCTIONS_H_
#define _COMMON_FUNCTIONS_H_

#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

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

enum BUF_SIZE {
    SMALL = 8,
    MEDIUM = 1024,
    BIG = 32768
};

int GetFSize(FILE *f);

int GetBufSize(int argc, char **argv);

#endif