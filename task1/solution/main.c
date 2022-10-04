#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "executer.h"

long long BUF_SZ = 2097152;


int main() {
    char string[BUF_SZ];
    char* str;
    While(1){
        int i = 0;
        char symb;
        symb = getchar();
        while (symb != '\n' && symb != EOF && i < BUF_SZ - 1) { //BUF_SZ-1 to garant that in buffer will be ending \0
            string[i] = symb;
            i++;
            symb = getchar();
        }
        string[i] = '\0';
        str = copy_cleaned(string, strlen(string));

        request_t** cmds = get_requests(str);
        execute(cmds);
        clean_reqs(cmds);
        free(str);
    }

    return 0;
}