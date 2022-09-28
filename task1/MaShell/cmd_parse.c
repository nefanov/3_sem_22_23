#include "MaShell.h"

void print_status(int status);
void parse_argv(token* cmd_line);

static size_t count_commands(const char* cmd_buffer)
{
    size_t cmd_num = 0;

    if (!*cmd_buffer) return 0; // No commands
    cmd_num++; // There is at least one

    while(*cmd_buffer) 
    {
        if (*cmd_buffer == '|')
        {
            cmd_num++;
        }

        cmd_buffer++;
    }

    return cmd_num;
}

void print_commands(token* cmd_array, size_t len)
{
    for (size_t cmd_id = 0; cmd_id < len; cmd_id++)
    {
        printf("[%lu] %s ", cmd_id, cmd_array[cmd_id].cmd);
        print_status(cmd_array[cmd_id].status);
        print_argv(cmd_array[cmd_id].argv);
        putchar('\n');
    }
}

void print_argv(char** argv)
{
    while(*argv) printf("%s ", *argv++);
}

#define PRINT_STATUS(TOKEN_STATUS) case(TOKEN_STATUS): printf(#TOKEN_STATUS " "); break
void print_status(int status)
{
    switch(status)
    {
    PRINT_STATUS(END);
    PRINT_STATUS(NOTH);
    PRINT_STATUS(LEFT);
    PRINT_STATUS(RIGHT);
    PRINT_STATUS(BOTH );
    }
}
#undef PRINT_STATUS

token* parse_cmd(char* cmd_buffer, size_t* cmd_num)
{
    *cmd_num = count_commands(cmd_buffer);

    token *cmd_array = (token*) calloc(*cmd_num + 1, sizeof (token));
    if (!cmd_array) PRINT_ERROR("Calloc returned NULL");
    
    size_t cmd_id = 0;
    char* command_ptr = NULL;

    command_ptr = strtok(cmd_buffer, "|"); // Crop trailing spaces
    while (command_ptr)
    {
        while(isspace(*command_ptr)) command_ptr++; // skip all space symbols

        if (!*command_ptr) 
        {
            printf("[ERROR]: Empty command\n");
            return (token*) ERROR;
        }

        cmd_array[cmd_id].status = BOTH; // Default value for token

        if (cmd_id == 0) cmd_array[cmd_id].status = RIGHT; // First element

        cmd_array[cmd_id++].cmd = command_ptr;
        parse_argv(&cmd_array[cmd_id-1]);

        command_ptr = strtok(NULL, "|");

        if (!command_ptr && cmd_id == 1) // The only command
            {
            cmd_array[cmd_id-1].status = NOTH;
            return cmd_array;
            }

        if (!command_ptr) cmd_array[cmd_id-1].status = LEFT; // Not only, last element
    }

    return cmd_array;
}

void parse_argv(token* cmd_line)
{
    char* begunok = (char*) cmd_line->cmd;
    size_t word_counter = 0;

    while(*begunok)
    {
        if (isalnum(*begunok) || ispunct(*begunok)) // We stay on command or its argument (some "word")
        {
            word_counter++;
        }

        while (!isspace(*begunok) && *begunok != '\0') begunok++; // Skip "word"

        while (isspace(*begunok)) begunok++; // Skip spaces
    }

    char** argv = (char**) calloc(word_counter + 1, sizeof(char*));
    if (!argv) PRINT_ERROR("Calloc returned zero");

    begunok = (char*) cmd_line->cmd;
    size_t argv_id = 0;
    while(*begunok)
    {
        if (isalnum(*begunok) || ispunct(*begunok)) // We stay on command or its argument (some "word")
        {
            argv[argv_id++] = begunok;
        }

        while (!isspace(*begunok) && *begunok != '\0') begunok++; // Skip "word"

        if (*begunok == '\0') // Last word without ahead spaces in cmd_line
        {
            break;
        }
        *begunok++ = '\0';

        while (isspace(*begunok)) begunok++; // Skip spaces
    }

    cmd_line->argv = argv;
}