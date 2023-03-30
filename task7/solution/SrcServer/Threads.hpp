#ifndef _H_THREADS_
#define _H_THREADS_

#include "Config.hpp"
#include "DebugFunctions.hpp"

struct ThreadData {
    int rFD;
    int wFD;
};

void* ThreadFunc(void* transmitData);

#endif