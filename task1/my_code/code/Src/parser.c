#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>


#include "pipe_manager.h"
#include "parser.h"


static const size_t START_STRIG_SIZE = 8;
static const size_t START_COMAND_SIZE = 2;
static const size_t START_ARGV_SIZE = 1;


static int cmd_str_realloc_up (comand* com);

static int comand_reader (comand* com);

static int cmd_call_mass_realloc_up (comand* com);

static int comand_parser (comand* com);

static int call_realloc_up (call* call);

static int call_parser (call* call);


static int cmd_str_realloc_up (comand* com)
{
    
    char* buf = NULL;
    
    buf = realloc (com->comand_string, (com->cmd_string_size * 2) * sizeof(char));
    
    if(buf == NULL)
    {
        printf("press f mem\n");
        return 1;
    }
    
    com->comand_string = buf;
    com->cmd_string_size *= 2;

    return 0;

} 

static int comand_reader (comand* com)
{
    
    com->comand_string = calloc(START_STRIG_SIZE, sizeof(char));
    com->cmd_string_size = START_STRIG_SIZE;

    int c_buf = 0;
    for(c_buf = getchar(); (c_buf != EOF) && (c_buf != '\n') && (c_buf != '\0'); c_buf = getchar())
    {
        
        *(com->comand_string + com->cmd_string_len) = c_buf;
        com->cmd_string_len ++;

        if (com->cmd_string_len == com->cmd_string_size)
        {
            cmd_str_realloc_up(com);
        }

    }

    *(com->comand_string + com->cmd_string_len) = '\0';
    com->cmd_string_len ++;
    
    return 0;

}

static int cmd_call_mass_realloc_up (comand* com)
{
    
    call* buf = NULL;
    
    buf = realloc (com->call_m, (com->cmass_size * 2) * sizeof(call) );
    
    if(buf == NULL)
    {
        printf("press f mem\n");
        return 1;
    }
    
    com->call_m = buf;
    com->cmass_size *= 2;

    return 0;

}

static int comand_parser (comand* com)
{
    
    char* buf = NULL;

    com->call_m = calloc(START_COMAND_SIZE, sizeof(call));
    com->cmass_size = START_COMAND_SIZE; 

    for(buf = strtok(com->comand_string, "|"); buf != NULL; buf = strtok(NULL, "|"))
    {
        
        (com->call_m + com->cmass_len)->argv = calloc(START_ARGV_SIZE, sizeof(char*));
        (com->call_m + com->cmass_len)->argsz = START_ARGV_SIZE;
        (com->call_m + com->cmass_len)->argc = 0;
        
        *((com->call_m + com->cmass_len)->argv) = buf;
        
        com->cmass_len ++;

        if (com->cmass_len == com->cmass_size)
        {
            cmd_call_mass_realloc_up(com);
        }

    }
    
    (com->call_m + com->cmass_len)->argv = NULL;
    (com->call_m + com->cmass_len)->argsz = 0;

    return 0;

}

static int call_realloc_up (call* call)
{
    
    char** buf = NULL;
    
    buf = realloc (call->argv, (call->argsz * 2) * sizeof(char*) );
    
    if(buf == NULL)
    {
        printf("press f mem\n");
        return 1;
    }
    
    call->argv = buf;
    call->argsz = call->argsz * 2;

    return 0;

}

static int call_parser (call* call)
{

    char* buf = NULL; 

    for(buf = strtok(*(call->argv), " "); buf != NULL; buf = strtok(NULL, " "))
    {
        
        
        *(call->argv + call->argc) = buf;
        call->argc ++;

        if (call->argc == call->argsz)
        {
            call_realloc_up (call);
        }

    }
    
    *(call->argv + call->argc) = NULL;

    return 0;

}

void comand_rp(comand* com)
{

    comand_reader(com);

    comand_parser(com);

    size_t i = 0;
    for(i = 0; i < com->cmass_len; i++)
    {
        call_parser(com->call_m + i);
    }
}

void cmd_struct_destructor (comand* com)
{
    size_t i = 0;
    
    for(i = 0; i < com->cmass_len; i++)
    {
        
        free((com->call_m + i)->argv);
        (com->call_m + i)->argsz = 0;
        (com->call_m + i)->argc = 0;

    }
    
    free(com->call_m);
    com->cmass_len = 0;
    com->cmass_size = 0;

    free(com->comand_string);
    com->cmd_string_size = 0;
    com->cmd_string_len = 0;
}
