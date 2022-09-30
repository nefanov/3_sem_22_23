#include "main.h"


char **parse(char *string)
{
    assert(string);

    int count, i, one_or_more; 
    char sep[10] = {};
    char **arr;
    one_or_more = 1;
    i = 0;

    while (string[i] != '\0')
    {
        if (string[i] == '|')
        {
            one_or_more = 2;
            break;
        }
        i++;
    }

    i = 0;

    if (one_or_more == 2)
    {
        strcpy(sep, "|");
    }
    else
    {
        strcpy(sep, " \n");
    }
    
    if (one_or_more == 2)
    {
        count = count_programms(string);
    }
    else
    {
        count = count_words(string);
    }

    arr = (char **)malloc((count + 1) * sizeof(char *));
    assert(arr);

    arr[i] = strtok (string,sep);

    while (arr[i] != NULL)
    { 
        i++;
        arr[i] = strtok (NULL,sep);
    }
    
    return arr;

}


int count_words(char *string)
{
    int count, len;
    count = 0;
    len = strlen(string);
    
    for (int i = 1; i < (len - 1); i++)
    {
        if (string[i - 1] != ' ' && string[i] == ' ')
        {
            count++;
        }
    }
    if (string[len - 2] != ' ')
    {
        count++;
    }

    return count;
}


int count_programms(char *line)
{
    int count, len;
    len = strlen(line);
    count = 1;

    for (int i = 0; i < len; i++)
    {
        if (line[i] == '|')
        {
            count++;
        }
    }

    return count;
}

