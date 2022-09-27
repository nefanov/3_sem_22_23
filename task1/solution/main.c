#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "executer.h"

long long BUF_SZ = 2097152;

int main() {
    char string[BUF_SZ];
    while (1) {
        int i = 0;
        char symb;
        symb = getchar();
        while (symb != '\n' && symb != EOF && i < BUF_SZ - 1) {
            string[i] = symb;
            i++;
            symb = getchar();
        }
        char* str = (char*) calloc(strlen(string) + 1, sizeof(char));
        strcpy(str, string);

        request_t** cmds = get_requests(string);
        execute(cmds);
        clean_reqs(cmds);
        free(str);
    }
    return 0;
}