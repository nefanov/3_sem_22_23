#ifndef TOKENIZATOR_H
#define TOKENIZATOR_H

#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <errno.h>


enum CMD
{
    MAX_NUM_TOKENS  = 256,
    CMD_MAX_SIZE    = 155,
};

enum CMD_ERRORS
{
    MAX_NUM_TOKENS_ERR = 0x00F,
    CONSTRUCT_ERR      = 0x01F,
    OK                 = 0x111,
};

struct CmdTokens
{
    char*  tokens[MAX_NUM_TOKENS + 1];
    char*** flags;

    int exitStatus[MAX_NUM_TOKENS];

    size_t tokensCapacity;
    size_t pipelineCapacity;

    size_t* flagsNum;
};

size_t tokenizator(struct CmdTokens* s_commands);

void inputFlags(struct CmdTokens* s_commands, size_t tokensCapcity);

size_t constructTokens(struct CmdTokens* s_commands);

void destructTokens(struct CmdTokens* s_commands);

void destructEmptyTokens(struct CmdTokens* s_commands);

char* strDuplicator(const char *start, const char *end);

char* godStrtok(char* string, char* delimeters, char** pos);

char* fixBeforeTok(char* token);

char* fixAfterTok(char* token, char* delim);

void printTokens(struct CmdTokens* s_commands);

#endif