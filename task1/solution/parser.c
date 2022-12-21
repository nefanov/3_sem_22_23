#include "parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct request_t_ {
    const char* command;
    char** args;
} request_t;

int is_sep(char elem, char* sep) {
    int i = 0;
    while (sep[i] != '\0') {
        if (sep[i] == elem) return 1;
        i++;
    }
    return 0; 
}

char* copy_cleaned(const char* buf, long unsigned buf_sz) {
    char* res = calloc(buf_sz + 1, sizeof(char));
    assert(res != NULL);
    long unsigned i = 0, j = 0;
    while(i < buf_sz && buf[i] != '\0') {
        if (i != 0) {
            if (is_sep(buf[i], " \t\n") && is_sep(buf[i - 1], " \t\n")) {
                i++;
                continue;
            }
        }
        res[j] = buf[i];
        i++;
        j++;
        
    }
    return res;
}

unsigned int count_chr(char* sep, const char* buf) {
    int i = 0;
    unsigned int cnt = 0;
    while (buf[i] != '\0') {
        if (is_sep(buf[i], sep)) cnt++;
        i++;
    }
    return cnt;
}

request_t **get_requests(char* reqs) {
    int i = 0, j = 0, n_reqs = count_chr("|", reqs) + 1, length = strlen(reqs);
    request_t** reqs_splitted = (request_t**) calloc(n_reqs + 1, sizeof(request_t*));
    while (i < length) {
        while (is_sep(reqs[i], " \t\n") && i < length) {
            reqs[i] = '\0';
            i++;
        }
        
        request_t* elem = (request_t*) calloc(1, sizeof(request_t));
        elem->command = &(reqs[i]);
        elem->args = split_cmds(&(reqs[i]));
        reqs_splitted[j] = elem;
        
        while (i < length && reqs[i] != '|') i++;
        if (reqs[i] == '|') reqs[i] = '\0';
        
        i++;
        j++;
    }
    reqs_splitted[j] = NULL;
    return reqs_splitted;

}

void dump_req(request_t* req) {
    printf("Command: %s \n", req->command);
    printf("Arguments: \n");
    int i = 0;
    while ((req->args)[i] != NULL) {
        printf("%s ", (req->args)[i]);
        i++;
    }
}

void dump_reqs_list(request_t** reqs) {
    int i = 0;
    while (reqs[i] != NULL) {
        dump_req(reqs[i]);
        printf("\n");
        i++;
    }
}

char** split_cmds(char* req) {
    char** requests = (char **) calloc(count_chr(" \t\n", req) + 2, sizeof(char*));
    int i = 0, j = 0;
    while (req[i] != '\0' && req[i] != '|'){
        requests[j] = &(req[i]);
        while (!is_sep(req[i], " \t\n") && req[i] != '\0' && req[i] != '|') i++;
        if (is_sep(req[i], " \t\n")) req[i] = '\0';
        i++;
        j++;
    }
    requests[j] = NULL;
    return requests;
}

const char* get_command(request_t* request) {
    return request->command;
}

char** get_args(request_t* request) {
    return request->args;
}

void clean_reqs(request_t** reqs) {
    int i = 0;
    while (reqs[i] != NULL) {
        free(reqs[i]->args);
        free(reqs[i]);
        i++;
    }
    free(reqs);
}