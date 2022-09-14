#ifndef TOKENIZATOR_H
#define TOKENIZATOR_H

#include <malloc.h>
#include <string.h>
#include <errno.h>


#define CMD_MAX_SIZE 256
#define MAX_NUM_CMD 16

struct CmdTokens
{
    char*  tokens[MAX_NUM_CMD];
    char** flags[MAX_NUM_CMD];

    size_t tokensCapacity;

    size_t commandLen[MAX_NUM_CMD];
    size_t flagsLen[MAX_NUM_CMD];
};

void tokenizator(struct CmdTokens* s_commands);

void constructTokens(struct CmdTokens* s_commands);

void destructTokens(struct CmdTokens* s_commands);

char* strDuplicator(const char *start, const char *end);

char* godStrtok(char* string, char* delimeters, char** pos);


#endif