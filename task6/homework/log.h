#ifndef LOG_H
#define LOG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

FILE* log;

#ifndef NDEBUG

#define $ fprintf(stderr, "LINE %d\n", __LINE__);

#endif

#define LOG_FUNC(function_name) fprintf(log, "CALLING FUNC: \'%s\' ON %d LINE\n", #function_name, __LINE__); \
                 fprintf(stderr, "ERROR: check log.txt\n");




#define LOG(msg) fprintf(log, "\nLOGGER_MSG\n"                         \
                              "MSG: %s\n"                              \
                              "FUNC:%s\n"  , msg, __FUNCTION__);         \



void closeLog();

void startLogging();

void showExample();

void logVar(const char* format, ...);


#define EXIT_ERROR -1337



#endif