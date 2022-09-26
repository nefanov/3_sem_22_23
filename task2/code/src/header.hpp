#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>


#ifndef HEADER_HPP
#define HEADER_HPP


struct Class_;


size_t get_filesize (FILE *in);
void coder (size_t num, char *buf);
size_t decoder (char *buf);
size_t receive (Class_ *class_, int fd);
size_t send    (Class_ *class_, int fd);


const size_t MAX_LENSIZE_BYTES = 11;



typedef struct {

    size_t (*rcv)(Class_ *, int) = receive;
    size_t (*snd)(Class_ *, int) = send;
    
} Interface;


typedef struct Class_ {

        char *buf = nullptr;   // intermediate buffer
        
        int fd_direct[2] {};    // array of r/w descriptors for "pipe()" call (for parent-->child direction)
        int fd_back  [2] {};      // array of r/w descriptors for "pipe()" call (for child-->parent direction)
        
        size_t len = 0;         // data length in intermediate buffer
        
        Interface interface {};
} Class_;


enum FD {

    STDIN   = 0,
    STDOUT  = 1,
    STDERR  = 2,
};


enum ERRORS {

    NO_ERROR        = 0,
    EXEC_CMD_ERROR  = 1,
    PIPE_ERROR      = 2,
    FORK_ERROR      = 3,
    OPEN_FILE_ERROR = 4,
    ALLOC_ERROR     = 5,
    READ_ERROR      = 6,
    WRITE_ERROR     = 7,

};

#endif