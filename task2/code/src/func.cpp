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


int send (Class_ *class_, int fd, int *error) {

    assert (error);
    assert (class_);
    assert (class_->buf);


    int tmp  = write (fd, class_->buf, class_->len);
    if (tmp != class_->len) {
        perror ("WRITE error");
        *error = WRITE_ERROR;
        return -1;
    }
    
    return tmp;
}


int receive (Class_ *class_, int fd, int *error) {

    assert (class_);
    assert (error);
    assert (class_->buf);
    
    int tmp  = read (fd, class_->buf, class_->len);
    if (tmp != class_->len) {
        perror ("READING BUF ERROR");
        *error = READ_ERROR;
        return -1;
    }

    return tmp;
}