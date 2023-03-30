#include "Threads.hpp"

void* ThreadFunc( void* transmitData ) {
    if( !transmitData ) {
        printf( "ERROR, DATA NULL PTR\n" );
        return NULL;
    }

    ThreadData* data = (ThreadData*) transmitData; 

    char cmd[MAX_SIZE] = {};
    char parse[MAX_SIZE] = {};

    int size = read(data->rFD, cmd, MAX_SIZE);
    ASSERTED(read, size, -1, NULL);

    sscanf(cmd, "%s", parse);

    if(!strcmp(parse, "GET")) {
        size -= 3;

        if (size <= 0) {
            write(data->wFD, "IDK", 4);

            fprintf(stderr, "Invalid input, no filename\n");
            return NULL;
        }

        sscanf(cmd + 3, "%s", parse );

        FILE *input = fopen(parse, "r");
        if (!input) {
            fprintf (stderr, "No such file\n");

            write(data->wFD, "IDK", 4);
            return NULL;
        }

        write(data->wFD, "ACK", 4);

        while((size = fread( parse, sizeof(char), MAX_SIZE - 1, input)) > 0) {
            parse[size] = '\0';
            
            write(data->wFD, parse, size + 1);
            
            read(data->rFD, parse, 3);
        }

        write(data->wFD, "ACK", 4);

        printf("Work is done\n");

        fclose( input );
        
        return NULL;
    }

    printf( "unknown command\n" );
    write( data->wFD, "IDK", 4 );
    return NULL;

}