#include "MaShell.h"

void print_status(int status);

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
        putchar('\n');
    }
}

#define PRINT_STATUS(TOKEN_STATUS) case(TOKEN_STATUS): printf(#TOKEN_STATUS); break
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

    token *cmd_array = (token*) calloc(*cmd_num, sizeof (token));
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
            return ERROR;
        }

        cmd_array[cmd_id].status = BOTH; // Default value for token

        if (cmd_id == 0) cmd_array[cmd_id].status = RIGHT; // First element

        cmd_array[cmd_id++].cmd = command_ptr;

        // while(!isspace(*command_ptr) && *command_ptr != '\0') // Crop ahead spaces
        // {
        //     command_ptr++;
        // }
        // *command_ptr = '\0';

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