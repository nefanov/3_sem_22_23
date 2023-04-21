#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct programma{
    char *name;
    char **args_array;
}programma;


typedef struct command_line{
    int count_programms;
    programma **progs;
}command_line;


command_line *create_command_line(char *string);
int is_it_again(char *string);
int count_programms(char *line);
int count_words(char *string);
char **parse(char *string);
int call_progs(command_line *line);
void destruct_command_line(command_line *line);

