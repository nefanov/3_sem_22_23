#pragma once



typedef struct request_t_ request_t;

request_t** get_requests(char* reqs);

const char* get_command(request_t* request);

char** get_args(request_t* request);

char** split_cmds(char* req);

void dump_reqs_list(request_t** reqs);

void dump_req(request_t* req);

void clean_reqs(request_t** reqs);

