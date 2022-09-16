#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <assert.h>


#ifndef HEADER_HPP
#define HEADER_HPP


enum MAX {
    
    CMD_MAX     = 256,
    OPT_MAX     = 48,
    WORD_MAX    = 256,
    LINE_MAX    = 256,
    PAGE_MAX    = 4096,
};


typedef struct BUF {

    char arr[PAGE_MAX] = {};

}BUF;

typedef struct CMD {

    char *opt [OPT_MAX] = {};

    size_t opt_num = 0;
    
}CMD;


typedef struct CMD_ARR {

    CMD cmd [PAGE_MAX] = {};

    size_t cmd_num = 0;

}CMD_ARR;



void input_cmd (CMD_ARR *arr, BUF *buf);
void clear_cmd (CMD_ARR *arr);
void exec_cmd  (CMD cmd);
bool is_tab (char ch, bool *status);


#endif