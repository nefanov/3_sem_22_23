#include "Handlers.hpp"


int RegHandle(int cmdFd, int ackFd, int* cmdSet, int* fileSet, int plc, struct pollfd* fds) {
    if (plc >= MAX_N) {
        printf( "Error, maximum of users is reached\n" );
        return 0;
    }

    char buf[MAX_SIZE] = {};
    char cmd[MAX_SIZE] = {};

    int result = read(cmdFd, buf, MAX_SIZE);
    ASSERTED(read, result, -1, -1);

    int size = strlen(buf), offset = 0;

    offset = skip_spaces(buf);

    sscanf(buf + offset, "%s", cmd);

    if (!strcmp(cmd, REG)) {
        offset += REG_SZ;
        offset += skip_spaces(buf + offset);
        
        if ((size - offset) <= 0) {
            printf("Invalid comand, fifo wasn't inputed\n" );

            write(ackFd, "IDK", 4);
            return 0;
        }
        
        sscanf(buf + offset, "%s", cmd); 
        mknod(cmd, S_IFIFO | 0666, 0);
        cmdSet[plc] = open( cmd, O_RDWR );

        if (cmdSet[plc] < 0) { 
            fprintf(stderr, "ERROR WHILE OPENING PIPE\n");
            
            write(ackFd, "IDK", 4);
            return 0;
        }
        
        offset += strlen(cmd);
        offset += skip_spaces(buf + offset);
        size -= offset;

        if (size <= 0) {
            close(cmdSet[plc]);
            
            
            printf ("Invalid cmd, file fifo wasn't inputed\n");
            
            write(ackFd, "IDK", 4);
            return 0;

        }

        sscanf(buf + offset, "%s", cmd);
        mknod(cmd, S_IFIFO | 0666, 0);
        fileSet[plc] = open( cmd, O_RDWR );

        if (fileSet[plc] < 0) { 
            close(cmdSet[plc]);

            fprintf(stderr, "ERROR WHILE OPENING PIPE\n");
            
            write(ackFd, "IDK", 4);
            return 0;

        }

        fds[plc].fd = cmdSet[plc];
        fds[plc].events = POLLIN;

        write(ackFd, "ACK", 4);
        
        return 1;
    } 
    
    write(ackFd, "IDK", 4);
    
    return 0;
}