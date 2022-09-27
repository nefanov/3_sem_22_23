#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <sys/stat.h>

#define MAX_BUFF 10240

typedef struct _Op_table op_table;

typedef struct _Pipe Pipe;

typedef struct _Op_table{
    
    size_t (*rcv)(Pipe *self);
    size_t (*snd)(Pipe *self);
} op_table;

typedef struct _Pipe {
    
    char *data_direct;
    char *data_back;
    int fd_direct[2];
    int fd_back[2];
    size_t length_direct;
    size_t length_back;
    op_table actions;
} Pipe;


size_t read_msg(Pipe *self) {
    
    assert(self);

    return read((self -> fd_direct)[0], self -> data_direct, self -> length_direct);

}

size_t send_msg(Pipe *self) {

    assert(self);

    return write((self -> fd_back)[1], self -> data_direct, self -> length_direct);

}

int pipe_ctor(Pipe *self) {

    if( !self ) {

        printf("ERROR, NULL PTR\n");
        return -1;
    }
    
    self -> data_direct = (char *)calloc(sizeof(char), MAX_BUFF);
    self -> data_back = (char *)calloc(sizeof(char), MAX_BUFF);
    
    if ( !self->data_back || !self -> data_direct){
        
        printf("ERROR WHILE ALOCCATING BUFFERS\n");
        return -1;
    }

    self -> length_direct = 0;
    self -> length_back = 0;
    
    if (pipe(self -> fd_direct) < 0) {
        
        printf("ERROR WHILE CREATING DIRECT PIPE\n");
        return -1;
    }
    
    if (pipe(self -> fd_back) < 0) {
        
        printf("ERROR WHILE CREATING BACK PIPE\n");
        return -1;
    }

    self -> actions.rcv = read_msg;
    self -> actions.snd = send_msg;
    return 0;

}

int main() {
    
    Pipe* duplex = (Pipe*)calloc(sizeof(Pipe), 1);
    if ( pipe_ctor(duplex) == -1) {
        
        printf("MAJOR ERROR, CAN NOT CRATE DUPLEX PIPE\n");
        return -1;
    } 

    const pid_t pid = fork();
    FILE *data = fopen("file.txt", "r");
    FILE* output = fopen("result.txt", "w");
    //FILE* log = fopen("log.txt", "w");
    struct stat size_buff = { };
    fstat ( fileno (data), &size_buff);
    size_t file_size = (size_t) (size_buff.st_size);


    if ( pid < 0 ) {

        printf("FORK FAILED\n");
        return -1;
    }

    if (pid) { //we are in parent
        close(duplex->fd_direct[1]);
        close(duplex->fd_back[0]);
        size_t size = 0;
        while(!feof(data)) {
        
            size = fread(duplex->data_direct, sizeof(char), MAX_BUFF, data );
            duplex->length_direct = size;
            duplex->actions.snd(duplex);
            duplex->actions.rcv(duplex);
            //fprintf(log, "size of answer [%lu]\n", duplex->length_direct);
            //fprintf(output, "%s", duplex->data_direct);
            fwrite(duplex->data_direct, sizeof(char), duplex->length_direct, output);

        }

        int status = 0;
        waitpid(pid, &status, 0);
    }
    else{
        int tmp[2] = {0, 0};
        tmp[0] = duplex->fd_direct[0];
        duplex->fd_direct[0] = duplex -> fd_back[0]; 
        duplex->fd_back[0] = tmp[0];
        tmp[1] = duplex->fd_direct[1];
        duplex->fd_direct[1] = duplex->fd_back[1];
        duplex->fd_back[1] = tmp[1];
        int i = 0;
        char* temp = NULL;
        temp = duplex->data_direct;
        duplex->data_direct = duplex->data_back;
        duplex->data_back = temp;
        close(duplex->fd_direct[1]);
        close(duplex->fd_back[0]);
        size_t size = 0;
        while((file_size) > 0) {
            
            duplex->length_direct = MAX_BUFF;
            size = duplex->actions.rcv(duplex);
            duplex->length_direct = size;
            file_size -= size;
            duplex->actions.snd(duplex);
        }
        exit(0);
    }
    fclose(output);
    fclose(data);
    return 0;

}