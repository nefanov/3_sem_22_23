#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stat.h>



#define BUFF_SIZE 4096



int main() {
    
    FILE* logfile = fopen("2_task_logfile.txt", "w");


    int forward_descrs[2] = {0};
    int backward_descrs[2] = {0};
    if(pipe(forward_descrs) != 0){
        fprintf(logfile, "LINE %d: (!) failed to create first pipe!\n", __LINE__);
        return -1;
    }
    if(pipe(backward_descrs) != 0){
        fprintf(logfile, "LINE %d: (!) failed to create second pipe!\n", __LINE__);
        return -1;
    }

    pid_t pid = fork();

    FILE *file_sended = fopen("2_task_reading.txt", "r");

    size_t filesize = 0;
    struct stat buff;

    stat("2_task_reading.txt", &buff);
    filesize = (size_t)buff.st_size;

    fprintf(logfile, "LINE %d: filesize is %zu\n", __LINE__, filesize);

    size_t parts = filesize / BUFF_SIZE;

    fprintf(logfile, "LINE %d:  %zu parts\n", __LINE__, parts);


    FILE *file_recieved = fopen("2_task_recieving.txt", "w");

    fprintf(logfile, "LINE %d: Succesful!\n", __LINE__);

    if (pid == 0) {
         // Child process
        close(forward_descrs[1]);
        close(backward_descrs[0]);

        char* child_recieving_data = (char*)calloc(BUFF_SIZE + 1, sizeof(char));

        if(child_recieving_data == NULL){
            fprintf(logfile, "LINE %d: failed to calloc!\n", __LINE__);
            return 0;
        }


        for(int i = 0; i < parts; i++){
            if(read(forward_descrs[0], child_recieving_data, BUFF_SIZE) == -1){
                // fprintf(logfile, "LINE %d: CHILD: failed to read at %d`nd part!\n", __LINE__, i);
                return 0;
            }else{
                // fprintf(logfile, "LINE %d: CHILD: succesful readed %d`nd part!\n", __LINE__, i);
            }

            fprintf(logfile, "LINE %d: CHILD: %d`nd part...\n", __LINE__, i);

            if(write(backward_descrs[1], child_recieving_data, BUFF_SIZE) == -1){
                // fprintf(logfile, "LINE %d: CHILD: (!) failed to write at %d`nd part!\n", __LINE__, i);
                return 0;
            }else{
                // fprintf(logfile, "LINE %d: CHILD: succesful writed at %d`nd part!\n", __LINE__, i);
            }
        }



    } else {
            // Parent process
        close(forward_descrs[0]);
        close(backward_descrs[1]);

        char* parent_temp_buffer = (char*)calloc(BUFF_SIZE + 1, sizeof(char));
        

        for(int i = 0; i < parts; i++){

            fread(parent_temp_buffer, 1, BUFF_SIZE, file_sended);

            if(write(forward_descrs[1], parent_temp_buffer, BUFF_SIZE) < BUFF_SIZE){
                // fprintf(logfile, "LINE %d: PARENT: failed to write to child at %d part!\n", __LINE__, i);
                return 0;
            }else{
                // fprintf(logfile, "LINT %d: PARENT: successful sended %d`nd part!\n", __LINE__, i);
            }

            fprintf(logfile, "LINE %d: PARENT: %i`nd part...\n", __LINE__, i);

            if(read(backward_descrs[0], parent_temp_buffer, BUFF_SIZE) == -1){
                // fprintf(logfile, "LINE %d: PARENT: failed to read from child at %d part!\n", __LINE__, i);
                return 0;
            }else{
                // fprintf(logfile, "LINE %d: PARENT: succesful readed %d`nd part!\n", __LINE__, i);
            }

            fwrite(parent_temp_buffer, 1, BUFF_SIZE, file_recieved);
        }

     }
    fclose(logfile);
    fclose(file_sended);
    fclose(file_recieved);
    return 0;
}