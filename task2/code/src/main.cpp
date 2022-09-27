#include <stdio.h>
#include <unistd.h>
#include "header.hpp"


int main (void) {

    Class_ class_ {};

/////////////////////////////////////////////////PIPE
    pipe (class_.fd_direct);            //direct (parent->read) (0)
    pipe (class_.fd_back);              //back   (parent -> write) (1)

    pid_t pid = 0;

/////////////////////////////////////////////////ALLOC MEM
    Class_ len = {};
    len.buf = (char *)calloc(MAX_LENSIZE_BYTES, sizeof (char));
    len.len = MAX_LENSIZE_BYTES;

    class_.buf = (char *)calloc (MAX_BUF_SIZE, sizeof (char));
    if (!class_.buf || !len.buf) {
        perror ("UNABLE TO ALLOC");
        _exit (ALLOC_ERROR);
    }

    int error = NO_ERROR;
    int tmp = 0;
    size_t gl_len  = 0;
    size_t cur_len = 0;


/////////////////////////////////////////////////FORK
    if ((pid = fork()) < 0) {
        perror ("FORK ERROR");
        return FORK_ERROR;
    }

    else if (pid == 0) {        

        error = close (class_.fd_direct[0]);
        error = close (class_.fd_back[1]);

        FILE *in = fopen ("./io/input", "rb");
        if  (!in) {
            perror ("UNABLE OPEN INPUT FILE");
            _exit (OPEN_FILE_ERROR);
        }
        FILE *out = fopen ("./io/output", "wb");
        if (!out) {
            perror ("UNABLE OPEN OUTPUT FILE");
            _exit  (OPEN_FILE_ERROR);
        }

/////////////////////////////////////////////////GET LEN
        gl_len = get_filesize (in);
        cur_len = MAX_LENSIZE_BYTES;


/////////////////////////////////////////////////SEND GLOBAL LEN
        coder (gl_len, len.buf);

        len.len = MAX_LENSIZE_BYTES;
        tmp = len.interface.snd (&len, class_.fd_direct[1], &error);
        if (tmp != cur_len) {
            fprintf (stderr, "SEND ERROR\n");
            fclose (in);
            fclose (out);
            _exit (error);
        }
/////////////////////////////////////////////////GET INPUT FILE SIZE

        while (gl_len > 0) {

/////////////////////////////////////////////////SEND CUR_LEN

            if (gl_len > MAX_BUF_SIZE)
                cur_len = MAX_BUF_SIZE;
            else
                cur_len = gl_len;
            gl_len -= cur_len;
            class_.len = cur_len;

            coder (cur_len, len.buf);


            tmp = len.interface.snd (&len, class_.fd_direct[1], &error);
            if (tmp != MAX_LENSIZE_BYTES) {
                fprintf (stderr, "SEND ERROR\n");
                fclose (in);
                fclose (out);
                _exit (error);
            }

/////////////////////////////////////////////////////READ INPUT FILE

            tmp = fread (class_.buf, sizeof (char), cur_len, in);
            if (tmp != class_.len) {
                perror ("READ error");
                fclose (in);
                fclose (out);
                _exit (READ_ERROR);
            }


    /////////////////////////////////////////////////SEND BUF & REC BUF

            tmp = class_.interface.snd (&class_, class_.fd_direct[1], &error);
            if (tmp != class_.len) {
                perror ("SEND ERROR");
                fclose (in);
                fclose (out);
                _exit (error);
            }

        
            tmp = class_.interface.rcv (&class_, class_.fd_back[0], &error);
            if (tmp != class_.len) {
                fclose (in);
                fclose (out);
                _exit (error);
            }
    /////////////////////////////////////////////////WRITE OUTPUT
            if (fwrite (class_.buf, sizeof (char), class_.len, out) != class_.len) {
                perror ("WRITE error");
                fclose (in);
                fclose (out);
                _exit  (WRITE_ERROR);
            }
        }

        fclose (in);
        fclose (out);
        _exit (NO_ERROR);
    }
    
    else {

        error = close (class_.fd_direct[1]);
        error = close (class_.fd_back[0]);


/////////////////////////////////////////////////RCV GLOBAL LEN
        
        tmp = class_.interface.rcv (&len, class_.fd_direct[0], &error);
            if (tmp != MAX_LENSIZE_BYTES)
                _exit (error);
        
        gl_len = decoder (len.buf);

        printf ("LEN: %u\n", gl_len);


/////////////////////////////////////////////////WHILE
        while (gl_len > 0) {
        
        /////////////////////////////////////////RCV LENGTH

            if (gl_len > MAX_BUF_SIZE)
                cur_len = MAX_BUF_SIZE;
            else
                cur_len = gl_len;
            gl_len -= cur_len;
            class_.len = cur_len;

            tmp = class_.interface.rcv (&len, class_.fd_direct[0], &error);
            if (tmp != MAX_LENSIZE_BYTES)
                _exit (error);
        
            tmp = decoder (len.buf);
            if (tmp != cur_len) {
                _exit (error);
            }


    /////////////////////////////////////////////////RCV BUF & SEND BUF
            tmp = class_.interface.rcv (&class_, class_.fd_direct[0], &error);
            if (tmp != class_.len)
                _exit (error);

            tmp = class_.interface.snd (&class_, class_.fd_back[1], &error);
            if (tmp != class_.len) {
                perror ("SEND ERROR");
                _exit (error);
            }

        }
/////////////////////////////////////////////////WAIT & CHECK STATUS
        int status = 0;
        waitpid (pid, &status, 0);

        if (status != 0) {

            fprintf (stderr, "STATUS: %d", status);
            return status;
        }

    }

    free (len.buf);
    free (class_.buf);

  return 0;
}