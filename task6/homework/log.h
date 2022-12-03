#ifndef LOG_H
#define LOG_H

#include <stdlib.h>
#include <stdio.h>

#define $ fprintf(stderr, "LINE %d\n", __LINE__);

void closeLog();

void startLogging();

void showExample();


#define EXIT_ERROR -1337

#endif