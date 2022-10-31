#include <stdio.h>
int main (void) {

    FILE * in = fopen ("kek", "wb");

    fwrite ("QWE", 1, 1, in);


}