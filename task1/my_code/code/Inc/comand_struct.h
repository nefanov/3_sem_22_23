

#ifndef COM_STRUCT
#define COM_STRUCT

typedef struct call
{
    size_t argsz;
    size_t argc;
    char** argv;
} call;

typedef struct comand
{
    size_t cmass_size;
    size_t cmass_len;

    size_t cmd_string_size;
    size_t cmd_string_len;
    
    call* call_m;
    char* comand_string;
} comand;
#endif