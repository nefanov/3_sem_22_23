#include "tokenizator.h"

void constructTokens(struct CmdTokens* s_commands)
{
    size_t tokensCapacity = 0;
    
    for (int i = 0; i < MAX_NUM_CMD; i++)
    {
        s_commands->tokens[i]  = NULL;
        s_commands->flags[i]   = NULL;

        s_commands->commandLen[i] = 0;
        s_commands->flagsLen[i]   = 0;
    }

    return;
}

void tokenizator(struct CmdTokens* s_commands)
{
    char delim[] = " ,-\n";

    static char cmd[CMD_MAX_SIZE] = {};

    fgets(cmd, CMD_MAX_SIZE, stdin);

    int cmdCounter = 0;
    char* p   = NULL;
    char* pos = NULL;

    for (p = godStrtok(cmd, delim, &pos); p != NULL;  p = godStrtok(NULL, delim, &pos))
    {
        s_commands->tokens[cmdCounter]     = p;
        s_commands->commandLen[cmdCounter] = strlen(p);
        cmdCounter++;
    }


    
}

void destructTokens(struct CmdTokens* s_commands)
{
    size_t counter = 0;

    while (s_commands->tokens[counter] != NULL)
    {
        free(s_commands->tokens[counter]);
        s_commands->tokens[counter] = NULL;
        counter++;
    }
    counter = 0;

    while (s_commands->flags[counter] != NULL)
    {
        free(s_commands->flags[counter]);
        s_commands->flags[counter] = NULL;
        counter++;
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

