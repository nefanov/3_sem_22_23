#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_CMD 1024
#define CMD_MAX_SIZE 2096640 

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
    
    char delim1[] = " \n";
    char delim2[] = "|\n";
    char *p;
    int i = 0, real_cmd_num = 1, j = 0, real_string_num = 1, cmd_size = 1;

    for(size_t k = 0; k < (strlen(buf) - 1); k++) {
    
        if(buf[k] == '|' && buf[k+1] != '|') cmd_size ++;
    
    }

    char** cmd_blocks = (char**)calloc(cmd_size, sizeof(char*));

    for(char *block = strtok(buf, delim2); block!=NULL; block = strtok(NULL, delim2)) {

        cmd_blocks[i] = strdup(block); 
        i++;

    }

    i = 0;
    (*cmd) = (char***)calloc(cmd_size + 1, sizeof(char**));

    for(; i < cmd_size; i++){
        
        for(char *p = strtok(cmd_blocks[i], delim1); p!=NULL; p = strtok(NULL, delim1)) {

            if(j == (MAX_CMD*(real_string_num -1 ))  ) {
        
                (*cmd)[i] = (char**)realloc((*cmd)[i], MAX_CMD*sizeof(char*)*real_string_num);
                real_string_num++;
            
            }

            (*cmd)[i][j] = strdup(p);
            j++;
        
        }

        if(j == (MAX_CMD*(real_string_num -1 ))  ) {
        
                (*cmd)[i] = (char**)realloc((*cmd)[i], MAX_CMD*sizeof(char*)*real_string_num + 1);
        
        }   
        
        (*cmd)[i][j] = NULL;
        j = 0;
        real_string_num = 1;

    }

    for(int k = 0; k < cmd_size; k++){
    
       free(cmd_blocks[k]);
    
    }
    
    free(cmd_blocks);
    (*cmd)[cmd_size] = NULL;

    return;
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
    
    printf( "[ %p ]", cmd[i]);
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
}

int main() {

    char *buf = (char*)calloc(CMD_MAX_SIZE, sizeof(char));
    fgets(buf, CMD_MAX_SIZE, stdin);
    
    while( strcmp(buf, "q\n") ){

        char ***cmd;

        parse_cmd(&cmd, buf);

        my_print_cmd(cmd);//printing parsed cmd for debug

        seq_pipe(cmd);
        free_cmd(cmd);
        free(cmd);
        fgets(buf, CMD_MAX_SIZE, stdin);

    }

    free(buf);
    return 0;
    
}