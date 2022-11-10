#ifndef _COMMON_FUNCTIONS_H_
#define _COMMON_FUNCTIONS_H_

#include <stdio.h>
#include <math.h>
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

const double EPS = 1e-06;

enum MLE {
    MORE,
    LESS,
    EQUAL
};

int CompDouble(const double a, const double b);

double DRand(const double min, const double max);

#endif