#include <MaShell.h>

char* findPathEnvVar(char* envp[]) // getenv
{
    const char search_pattern[] = "PATH=";
    const size_t pattern_len = sizeof search_pattern / sizeof search_pattern[0] - 1; // not consider '\0'

    while(*envp)
    {
        size_t letter_id = 0;

        for (; letter_id < pattern_len; letter_id++)
        {
            if (search_pattern[letter_id] != (*envp)[letter_id]) break;
        }

        if (letter_id == pattern_len) return *envp;

        envp++;
    }

    return NULL;
}