#include "../SrcServer/Config.hpp"

int skip_spaces(char *str){
    int i = 0;
    while( str[i] != '\0' && isspace(str[i])) i++;

    return i;
}

int main() {
    mknod(CMD_FIFO, S_IFIFO | 0666, 0);
    
    int cmdFd = open( CMD_FIFO, O_WRONLY ); 
    if (cmdFd < 0) {
        fprintf (stderr, "ERROR WHILE CREATING NAMED PIPE\n");
        
        return -1;
    }

    mknod( ACK_FIFO, S_IFIFO | 0666, 0 );
    
    int ackFd = open( ACK_FIFO, O_RDONLY );
    if (ackFd < 0) { 
        fprintf (stderr, "ERROR WHILE CREATING NAMED PIPE\n");
        
        return -1;
    }

    char cmd[MAX_SIZE] = {};

    printf("Please, register to the server\nTo do it type REGISTER \
    <file/tx/path> <fifo/rx/path> on your keyboard\n");

    fgets(cmd, MAX_SIZE, stdin );

    char buf[MAX_SIZE] = {};

    int offset = 0, size = 0;

    write(cmdFd, cmd, MAX_SIZE);
    read(ackFd, buf, MAX_SIZE);
    
    if (strcmp( buf, "ACK")) { 
        fprintf(stderr, "error, bad input\n");
        return -1;

    }

    offset = skip_spaces(cmd);
    
    sscanf(cmd, "%s", buf);
    
    offset += REG_SZ;
    offset += skip_spaces(cmd + offset);
    
    sscanf(cmd + offset, "%s", buf);

    mknod(buf, S_IFIFO | 0666, 0);
    
    int wFD = open(buf, O_WRONLY);
    if (wFD < 0) {
        fprintf(stderr, "error while opening pipe\n");
        
        return -1;
    }
    
    offset += strlen(buf);

    sscanf(cmd + offset, "%s", buf);
    
    mknod(buf, S_IFIFO | 0666, 0);

    int rFD = open(buf, O_RDONLY);
    if (rFD < 0) {
        close(wFD);

        fprintf(stderr, "error while opening pipe\n");
        
        return -1;
    }

    printf("Registration is successful\n");

    while (1) {
        fgets(cmd, MAX_SIZE, stdin);

        write(wFD, cmd, MAX_SIZE);
        read(rFD, cmd, 4);

        if (!strcmp( cmd, "ACK")) {
            while ((size = read(rFD, cmd, MAX_SIZE )) > 0) {
                if (!strcmp(cmd, "ACK")){
                    printf( "That's it\n");
                    
                    break;
                }
                printf("%s\n", cmd);
                
                write(wFD, "ACK", 3);
            }
        }
    }
    return 0;
}