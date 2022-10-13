#include "duplex.h"
#ifndef SIZE

#define SIZE 8192

#endif

int main(int argc, char *argv[]) {
    //making in/out files
    char* read_name = "data.txt";
    char* write_name = "out.txt";
    switch (argc) {
        case 1:
            printf("You didn`t typed fnames, set default\n");
            break;
        case 3:
            read_name = argv[1];
            write_name = argv[2];
            break;
        default:
            printf("Unkown type of arguments\n");
            exit(1);
    }
    OpTable* operations = OpTableConstructor(recieve, send, write_f, read_f, open_f, close_f, set_status);
    Pipe* pipe = PipeConstructor(SIZE, operations);
    pipe->actions->open_f(pipe, read_name, write_name);
    //make fork
    pid_t pid = fork();
    if (pid == 0) {
        //child
        pipe->status = CHILD;
        while (pipe->actions->recieve(pipe) != 0) {
            pipe->actions->send(pipe);
        }


    } else if (pid > 0) {
        //parent
        pipe->status = PARENT;
        while (pipe->actions->read_f(pipe) != 0) {
            pipe->actions->send(pipe);
            pipe->actions->recieve(pipe);
            pipe->actions->write_f(pipe);
        }


    } else {
        perror("fork error");
        exit(1);
    }

    PipeDestructor(pipe);
    return 0;
}
