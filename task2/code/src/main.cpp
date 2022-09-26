#include <stdio.h>
#include <unistd.h>
#include "header.hpp"


int main (void) {

    Class_ class_ {};

    pipe (class_.fd_direct);            //direct (parent->read) (0)
    pipe (class_.fd_back);              //back   (parent -> write) (1)

    pid_t pid = 0;


    if ((pid = fork()) < 0) {
        perror ("FORK ERROR");
        return FORK_ERROR;
    }

    else if (pid == 0) {
        
        int error = 0;
        error = close (class_.fd_direct[0]);
        error = close (class_.fd_back[1]);

        FILE *in = fopen ("./io/input", "rb");
        if (!in) {
            perror ("UNABLE OPEN INPUT FILE");
            _exit (OPEN_FILE_ERROR);
        }

/////////////////////////////////////////////////GET INPUT FILE SIZE
        class_.len = get_filesize (in);

        fprintf (stderr, "LEN: %lu\n", class_.len);

        class_.buf = (char *)calloc (class_.len + MAX_LENSIZE_BYTES, sizeof (char));
        if (!class_.buf) {
            perror ("UNABLE TO ALLOC");
            _exit (ALLOC_ERROR);
        }

/////////////////////////////////////////////////////READ INPUT FILE
        coder (class_.len, class_.buf);


        if (fread (class_.buf + MAX_LENSIZE_BYTES, sizeof (char), class_.len, in) != class_.len) {
            perror ("READ error");
            _exit (READ_ERROR);
        }

/////////////////////////////////////////////////ACTIONS

        error = class_.interface.snd (&class_, class_.fd_direct[1]);
        if (error != NO_ERROR)
            _exit (error);

        error = class_.interface.rcv (&class_, class_.fd_back[0]);
        if (error != NO_ERROR)
            _exit (error);
    
/////////////////////////////////////////////////WRITE OUTPUT
        FILE *out = fopen ("./io/output", "wb");
        if (!out) {
            perror ("UNABLE OPEN OUTPUT FILE");
            _exit  (OPEN_FILE_ERROR);
        }

        if (fwrite (class_.buf + MAX_LENSIZE_BYTES, sizeof (char), class_.len, out) != class_.len) {
            perror ("WRITE error");
            _exit  (WRITE_ERROR);
        }


        fclose (in);
        fclose (out);

        _exit (NO_ERROR);
    }
    
    else {

        int error = NO_ERROR;
        error = close (class_.fd_direct[1]);
        error = close (class_.fd_back[0]);


/////////////////////////////////////////////////ACTIONS
        error = class_.interface.rcv (&class_, class_.fd_direct[0]);
        if (error != NO_ERROR)
            exit (error);

        error = class_.interface.snd (&class_, class_.fd_back[1]);
        if (error != NO_ERROR)
            exit (error);

/////////////////////////////////////////////////WAIT
        int status = 0;
        waitpid (pid, &status, 0);

        if (status != 0) {

            fprintf (stderr, "STATUS: %d", status);
            return status;
        }
    }

  return 0;
}