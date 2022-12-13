#include "main.h"


char **parse(char *string)
{
    assert(string);

    int count, elem, one_or_more; 
    char sep[10] = {};
    char **arr;
    one_or_more = 1;
    elem = 0;

    while (string[elem] != '\0')
    {
        if (string[elem] == '|')
        {
            one_or_more = 2;
            break;
        }
        elem++;
    }

    elem = 0;

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

    arr[elem] = strtok (string,sep);

    while (arr[elem] != NULL)
    { 
        elem++;
        arr[elem] = strtok (NULL,sep);
    }
    
    return arr;

}


int count_words(char *string)
{
    assert(string);

    int count, len;
    count = 0;
    len = strlen(string);
    
    for (int elem = 1; elem < (len - 1); elem++)
    {
        if (string[elem - 1] != ' ' && string[elem] == ' ')
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
    assert(line);

    int count, len;
    len = strlen(line);
    count = 1;

    for (int elem = 0; elem < len; elem++)
    {
        if (line[elem] == '|')
        {
            count++;
        }
    }

    return count;
}

