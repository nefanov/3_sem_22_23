#pragma once

/*
for FIFO part I need:

-sender: 

    mkfifo, sync fifo name, write to FIFO as file

-receiver

    read from FIFO

-file: can be generated with cmd command from prev task
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define FIFO_NAME "ma_fifo"
#define BUFFER_SIZE 1024

#define INPUT_NAME "input.txt"
#define OUTPUT_NAME "output.txt"

void logr (const char* format, ...);
void logs (const char* format, ...);