#pragma once

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define CHILD 0
#define ERROR -1

enum
{
    READ_FD  = 0,
    WRITE_FD = 1
};

#define LOG_ERROR(msg) log("[ERROR] In %s function %s line %d:\n\t%s",\
                                __FILE__, __func__, __LINE__, msg);

#define CATCH(msg, ...)                 \
    do{                                 \
    int ret = __VA_ARGS__;              \
    if (ret == ERROR)                   \ 
    {                                   \
        LOG_ERROR(msg);                 \
        return ERROR;                   \
    }                                   \
    } while(0);

#define NDEBUG
#ifdef NDEBUG

    void logp (const char* format, ...);
    void logc (const char* format, ...);
    void log (const char* format, ...);

#else

    #define logp(...)
    #define logc(...)
    #define log(...)

#endif
