#include "header.hpp"


char *look_tab (char *strt, bool *status) {

    char *end = strt;

    while (1) {

        if (is_tab (*end, status)) {
            
            *end = '\0';

            if (!is_tab (*(end + 1), status))
                return end;
        }

        ++end;
    }
}


bool is_tab (char ch, bool *status) {

    assert (status);


    switch (ch) {

        case ' ':
            return true;
        break;

        case '\n':
            return true;
        break;

        case '|':
            *status = true;
            return true;
        break;

        default:
            return false;
        break;
    }
}


void input_cmd (CMD_ARR *arr, BUF *buf) {
    
    assert (arr);
    assert (buf);


    fgets (buf->arr, OPT_MAX, stdin);

    char *strt = buf->arr;
   
    while (1) {

        bool status = false;

        char *end = look_tab (strt, &status) + 1;

        arr->cmd[arr->cmd_num].opt[arr->cmd[arr->cmd_num].opt_num++] = strt;
            
        if (*end == '\0' || status) {

            arr->cmd[arr->cmd_num].opt[arr->cmd[arr->cmd_num].opt_num] = (char *)NULL;

            if (status) {
                ++arr->cmd_num;
                strt = end;
            }
            else
                return;
        }
        else {
            strt = end;
        }        
    }
}


size_t exec_cmd (CMD cmd) {

    if (execvp (cmd.opt[0], cmd.opt) == -1)
        return EXEC_CMD_ERROR;
    
    return NO_ERROR;
}


void clear_cmd (CMD_ARR *arr) {

    assert (arr);

    for (size_t i = 0; i <= arr->cmd_num; ++i) {
        for (size_t j = 0; j < arr->cmd[i].opt_num; ++j) {
            arr->cmd[i].opt[j] = nullptr;
        }
        arr->cmd[i].opt_num = 0;
    }
    arr->cmd_num = 0;
}