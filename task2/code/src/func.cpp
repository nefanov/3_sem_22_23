#include "header.hpp"


void coder (size_t num, char *buf) {

    assert (buf);
    
    size_t i = 0;

    while (num != 0) {
            
        int cur = num % 10;

        buf[i] = '0' + cur;

        num /= 10;         

        ++i;
    }

    buf[i] = '\0';

}


size_t decoder (char *buf) {

    assert (buf);

    size_t num = 0;
    size_t mul = 1;
    size_t i   = 0;

    while (buf[i] != '\0') {
        num = num + mul * (buf[i] - '0');
        mul *= 10;
        ++i;
    }

    return num;
}


size_t get_filesize (FILE *in) {

    assert (in);

    struct stat buff = {};

    fstat (fileno (in), &buff);

    return buff.st_size;
}


size_t send (Class_ *class_, int fd) {

    assert (class_);
    assert (class_->buf);

    size_t written = 0;

    while (1) {

    
        int tmp = write (fd, class_->buf + written, class_->len + MAX_LENSIZE_BYTES);
                
        if (tmp == -1) {
            fprintf (stderr, "WRITTEN %lu MUST WRITE %lu", written, class_->len + MAX_LENSIZE_BYTES);
            perror ("WRITE error");
            return WRITE_ERROR;
        }

        written += tmp;

        if (written == class_->len + MAX_LENSIZE_BYTES) {
            break;
        }
    }

    return NO_ERROR;
}


size_t receive (Class_ *class_, int fd) {

    assert (class_);

    char buf_len [MAX_LENSIZE_BYTES] = {};
    
    if (read (fd, buf_len, MAX_LENSIZE_BYTES) != MAX_LENSIZE_BYTES) {
        perror ("READING LEN ERROR");
        _exit (READ_ERROR);
    }

    class_->len = decoder (buf_len);

    if (class_->buf != nullptr) {
        free (class_->buf);
        class_->buf = nullptr;
    }
/////////////////////////////////////////////////
    class_->buf = (char *)calloc (class_->len + MAX_LENSIZE_BYTES, sizeof (char));
    
    memcpy (class_->buf, buf_len, MAX_LENSIZE_BYTES);
    
    size_t cnt = 0;
    while (1) {
    
        int tmp = read (fd, class_->buf + cnt + MAX_LENSIZE_BYTES, class_->len);


        if (tmp == -1) {
            perror ("READING BUF ERROR");
            return READ_ERROR;
        }

        cnt += tmp;
        if (cnt == class_->len) {
            break;
        }
    }

    return NO_ERROR;
}