#include "parsing.hpp"

size_t get_n_words(const char *token) {
    size_t n_words = 0;
    size_t len = strlen(token);

    for (size_t i = 1; i < len; i++) {
        if (token[i - 1] == ' ' && token[i] != ' ')
            n_words++;
    }

    if (token[0] != ' ')
        n_words++;

    return n_words;
}

char **get_tokens(char *str) {
    assert(str);

    size_t len = strlen(str);
    size_t n_prsses = get_n_processes(str);

    char **tokens = (char **) calloc(n_prsses, sizeof(char *));
    assert(tokens);
    
    tokens[0] = str;

    size_t j = 1;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '|') {
            str[i] = 0;
            tokens[j] = &str[i + 1];
            j++;
        }
    }

    return tokens;
}

struct process get_process_info(char *token) {
    assert(token);

    size_t n_args = get_n_words(token) - 1;

    char delim[] = " \n";

    struct process prss;
    prss.name = strtok(token, delim);

    char **args = (char **) calloc(n_args, sizeof(char *));

    for (size_t i = 0; i < n_args; i++) {
        args[i] = strtok(NULL, delim);
    }

    prss.args = args;

    return prss;
}

size_t get_n_processes(const char *str) {
    size_t n_process = 1;
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++) {
        if (str[i] == '|') {
            n_process++;
        }
    }

    return n_process;
}

struct command_line parse(char *str) {
    size_t n_prsses = get_n_processes(str);
    
    command_line cmd;
    cmd.prss = (struct process *) calloc(n_prsses, sizeof(struct process));
    assert(cmd.prss);

    char **tokens = get_tokens(str);

    for (size_t i = 0; i < n_prsses; i++) {
        cmd.prss[i] = get_process_info(tokens[i]);

        // printf("%d\n", i);
    }

    return cmd;
}
