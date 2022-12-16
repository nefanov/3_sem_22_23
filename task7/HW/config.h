#ifndef INCLUDE
#define INCLUDE

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <pthread.h>
#include <ctype.h>

#define DEF_KEY 0xcafebabe
#define CMD_FIFO "cmd.fifo"
#define ACK_FIFO "ack.fifo"
#define MAX_N 64
#define MAX_SIZE 1024
#define REG "REGISTER"
#define REG_SZ 8
#define TIMEOUT 1000

int skip_spaces(char *str){
    
    int i = 0;
    while( str[i] != '\0' && isspace(str[i])) i++;

    return i;
}

#endif