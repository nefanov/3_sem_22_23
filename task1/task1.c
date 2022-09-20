#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

#define MAX_CMD 1024
#define CMD_MAX_SIZE 4096

void seq_pipe(char*** cmd) {
    int p[2];
    pid_t pid;
    int fd_in = 0, i = 0;
    while(cmd[i] != NULL) {
        pipe(p);
        if( (pid = fork()) == -1 ){
            printf("Fork error\n");
            exit(1);
        } else {
            if ( pid == 0) {
                if( i > 0) dup2(fd_in, 0); //stdin switched to fd_in
                if (cmd[i + 1] != NULL) dup2(p[1], 1); //if next cmd not last stdout switched to pid out 
                close(p[0]); //closing read
                execvp((cmd)[i][0], cmd[i]);
                printf("exec failed\n"); //if we are here file wasn't executed
                exit(2);
            } else {
                int status = 0;
                waitpid(pid, &status, 0);
                printf("Ret code: %d\n", WEXITSTATUS(status));//printing exit status for debug
                close(p[1]);
                if (i > 0) close(fd_in); //closing fd_in
                fd_in = p[0]; //fd_in equals previous read
                i++;
            }
        }
    }
    return;
}


void print_cmd();

void parse_cmd(char **** cmd, char *buf){
    char delim[] = " \n";
    char *p;
    int i = 0, real_cmd_num = 1, j = 0, real_string_num = 1;

    //(*cmd)[0] = (char **)calloc(MAX_CMD, sizeof(char*));

    //printf("This is the input :%s\n", buf);

    for (char *p = strtok(buf, delim); p!=NULL; p = strtok(NULL, delim) ) {
        if( i == (MAX_CMD * (real_cmd_num - 1))) {
            
            (*cmd) = (char***)realloc((*cmd), MAX_CMD*sizeof(char**) * real_cmd_num);
            //printf("realoccing cmd\n");
            real_cmd_num++;
        }

        if ( j == (MAX_CMD*(real_string_num - 1))) {
            (*cmd)[i] = (char**)realloc((*cmd)[i], MAX_CMD*sizeof(char*)*real_string_num);
            //printf("reallocing string\n");
            real_string_num++;
        }

        if ( !strcmp( p, "|") ){
            (*cmd)[i][j] = NULL;
            //printf("Facing |, putting NULL\n");
            j = 0;
            real_string_num = 1;
            i++; 
            //(*cmd)[i] = (char **)calloc(MAX_CMD, sizeof(char*));
        } else {

            //printf("adding string\n");
            //printf("string is %s\n", p);
            //printf("last symbol code %d\n", p[strlen(p)]);
            //printf("i index is %lu, j index is %lu\n", i, j);
            //printf("ptr cmd %p\n", (*cmd)[i]);
            (*cmd)[i][j] = strdup(p);
            //printf("string is %s\n", (*cmd)[i][j] );
            //print_cmd(*cmd);
            //printf("string added\n");

            j++;
        }


    }
    if ( j == (MAX_CMD*(real_string_num - 1))) {
        (*cmd)[i] = (char**)realloc((*cmd)[i], MAX_CMD*sizeof(char*)*real_string_num);
            //printf("reallocing string\n");
            //real_string_num++;
    }
    (*cmd)[i][j] = NULL;
    i++;
    if( i == (MAX_CMD * (real_cmd_num - 1))) {
            
            (*cmd) = (char***)realloc(*cmd, MAX_CMD*sizeof(char**) * (real_cmd_num - 1) + 1);
            //printf("realoccing cmd\n");
            //printf("is it null terminated ? %s", (*cmd)[i]);
    }
    (*cmd)[i] = NULL;

    return;
}


void print_cmd(char ***cmd) {
    size_t i = 0;
    for (; cmd[i] != NULL; i++){
        size_t j = 0;
        for(/*size_t j = 0*/; (cmd)[i][j] != NULL; j++){
            for(size_t k = 0; k < i; k++) printf("%c", '\t');
            printf(" i is %lu j is %lu, %s\n",i, j, (cmd)[i][j]);
            //size_t str_len = strlen((cmd)[i][j]);
            //printf("%d\n", (cmd)[i][j][str_len]);
        }
        for(size_t k = 0; k < i; k++) printf("%c", '\t');
        printf(" The end of the char** %s\n", (cmd)[i][j]);
    }
    printf(" the end of the char *** %p\n", (cmd)[i]);
    //printf("code of 0: %d", '\0');
}

void my_print_cmd(char ***cmd){
    size_t i = 0;
    for(; (cmd)[i] != NULL; i++) {
        printf("[");
        size_t j = 0;
        for(; (cmd)[i][j] != NULL; j++) {
            printf("%s ", (cmd)[i][j]);
        } printf("%s ", (cmd)[i][j]);
        printf("] ");
    } 
    printf( "[ %s ]", cmd[i]);
    printf("\n");
}

void free_cmd(char ***cmd){
    size_t i = 0;
    for(; (cmd)[i] != NULL; i++) {
        size_t j = 0;
        for(; (cmd)[i][j] != NULL; j++) {
            free((cmd)[i][j]);
        } free((cmd)[i][j]);
        free(cmd[i]);
    } 
    free(cmd[i]);
    //printf("\n");
}

int main() {
    char *buf = (char*)calloc(CMD_MAX_SIZE, sizeof(char));
    fgets(buf, CMD_MAX_SIZE, stdin);
    
    while( strcmp(buf, "q\n") ){
    
         //while(getchar() != '\n'){}

        char ***cmd = (char***)calloc(MAX_CMD, sizeof(char**));

        parse_cmd(&cmd, buf);

        //free(buf);

        my_print_cmd(cmd);//printing parsed cmd for debug

        seq_pipe(cmd);
        free_cmd(cmd);
        free(cmd);
        fgets(buf, CMD_MAX_SIZE, stdin);
    }
    free(buf);
    return 0;
}