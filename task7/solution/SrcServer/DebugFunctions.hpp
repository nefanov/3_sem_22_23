#ifndef _H_DEBUG_FUNCTIONS_
#define _H_DEBUG_FUNCTIONS_

#include <string.h>
#include <stdio.h>

#define ASSERTED(func, var, cntrlValue, retValue)                                                                                                                       \
    if (strcmp(#func, "NP") == 0) {                                                                                                                                     \
        printf("An exception occurred with variable "#var" in function %s() at file %s on line %d\n", __func__, __FILE__, __LINE__ - 1);                                \
        if (var == cntrlValue) {                                                                                                                                        \
            perror("null pointer passed");                                                                                                                              \
                                                                                                                                                                        \
            return retValue;                                                                                                                                            \
        }                                                                                                                                                               \
    } else if (var == cntrlValue) {                                                                                                                                     \
        printf("An exception occurred with variable "#var" in function %s() at file %s on line %d\n", __func__, __FILE__, __LINE__ - 1);                                \
        perror(#func"() failed");                                                                                                                                       \
        return retValue;                                                                                                                                                \
    }

#define $ printf("I'm here %d\n", __LINE__);

#endif