#include "tokenizator.h"

size_t constructTokens(struct CmdTokens* s_commands)
{
    assert(s_commands);

    s_commands->tokensCapacity   = 0;
    s_commands->pipelineCapacity = 0;
    s_commands->flagsNum         = NULL;

    for (int i = 0; i < MAX_NUM_TOKENS; i++)
    {
        s_commands->tokens[i]  = NULL;
    }

    return OK;
}

size_t tokenizator(struct CmdTokens* s_commands)
{
    assert(s_commands);

    char delim[] = " ,;\n";

    char cmd[CMD_MAX_SIZE] = {};

    fgets(cmd, CMD_MAX_SIZE, stdin);

    size_t cmdCounter = 0;
    char* token       = NULL;
    char* pos         = NULL;

    for (token = godStrtok(cmd, delim, &pos); ((token!=NULL)&(cmdCounter<MAX_NUM_TOKENS)); token = godStrtok(NULL, delim, &pos))
    {
        s_commands->tokens[cmdCounter] = token;
        
        if (strcmp("|", token) == 0)
        {
            s_commands->pipelineCapacity++;
        }

        cmdCounter++;
    }

    if (cmdCounter == MAX_NUM_TOKENS)
    {
        s_commands->tokensCapacity = MAX_NUM_TOKENS + 1;
        return MAX_NUM_TOKENS_ERR;
    }

    s_commands->tokensCapacity = cmdCounter;

    s_commands->flagsNum = (size_t*)calloc(s_commands->pipelineCapacity + 1, sizeof(size_t));
    s_commands->flags    = (char***)calloc(s_commands->pipelineCapacity + 1, sizeof(char***));

    inputFlags(s_commands, cmdCounter);

    return OK;
}


void inputFlags(struct CmdTokens* s_commands, size_t tokensCapcity)
{
    assert(s_commands);

    size_t flagsNum = 0;
    size_t pipelineCounter = 0;

    for (size_t flagsCounter = 0; flagsCounter < tokensCapcity; flagsCounter++)
    {
        if (strcmp(s_commands->tokens[flagsCounter], "|") == 0)
        {
            s_commands->flagsNum[pipelineCounter] = flagsNum;
            pipelineCounter++;
            flagsNum = 0;
        }
        else
        {
            flagsNum++;
        }
    }

    s_commands->flagsNum[pipelineCounter] = flagsNum;

    flagsNum = 0;
    size_t tokensCounter = 0;
    
    for (size_t flagsCounter = 0; flagsCounter < s_commands->pipelineCapacity + 1; flagsCounter++)
    {
        s_commands->flags[flagsCounter] = calloc(s_commands->flagsNum[flagsCounter] + 1, sizeof(char*));

        for (size_t i = 0; i < s_commands->flagsNum[flagsCounter]; i++)
        {
            s_commands->flags[flagsCounter][i] = s_commands->tokens[tokensCounter];
            tokensCounter++;
        }
        s_commands->flags[flagsCounter][s_commands->flagsNum[flagsCounter]] = NULL;

        tokensCounter++;
    }
}

void destructTokens(struct CmdTokens* s_commands)
{
    assert(s_commands);

    for (size_t counter = 0; counter < s_commands->tokensCapacity; counter++)
    {
        free(s_commands->tokens[counter]);
        s_commands->tokens[counter] = NULL;
    }

    for (size_t counter = 0; counter < s_commands->pipelineCapacity + 1; counter++)
    {
        free(s_commands->flags[counter]);
        s_commands->flags[counter] = NULL;
    }

    free(s_commands->flags);
    s_commands->flags = NULL;

    free(s_commands->flagsNum);
    s_commands->flags = NULL;
}

void destructEmptyTokens(struct CmdTokens* s_commands)
{
    for (size_t counter = 0; counter < s_commands->tokensCapacity; counter++)
    {
        free(s_commands->tokens[counter]);
        s_commands->tokens[counter] = NULL;
    }
}


char* strDuplicator(const char* start, const char* end)
{
  size_t lenStr = end - start;

  char* dup = calloc(lenStr + 1, sizeof(char));

  if (dup != NULL) 
  {
    strncpy(dup, start, lenStr);
  }

  return dup;
}

char* godStrtok(char* string, char* delimeters, char** pos)
{
    if (string == NULL)
    {
        string = *pos;
    }

    char* walker = string;
    char* token  = NULL;

    while (*walker != '\0')
    {
        if (strchr(delimeters, *walker) == 0)
        {
            walker++;
        }
        else
        {
            token = strDuplicator(string, walker);
            *pos = walker + strspn(walker, delimeters);
            break;
        }
    }
    return token;
}


void printTokens(struct CmdTokens* s_commands)
{
    for (size_t i = 0; i < s_commands->pipelineCapacity + 1; i++)
    {
        printf("flags in %lu pipeline: %lu;\nflags: ", i + 1, s_commands->flagsNum[i]);

        for (size_t j = 0; j < s_commands->flagsNum[i]; j++)
        {
            printf("%s ", s_commands->flags[i][j]);
        }
        printf("\n");
    }

    for (size_t i = 0; i < s_commands->pipelineCapacity + 1; i++)
    {
        printf("exit status of [%lu] process:%d\n", i, s_commands->exitStatus[i]);
    }

}