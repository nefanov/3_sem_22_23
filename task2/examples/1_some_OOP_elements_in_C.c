/* Tiny example of function table usage as structure field -- implementation of incapsulation and polymorphism
OOP principles in clearly procedural languages (like C). Oftenly used in system programming */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SZ 4096

typedef struct pString String;

typedef struct op_table Ops;

typedef struct op_table  {
    int (*len)(String *self); // "op_table" usually named in file systems; 
                              // "priv" usually named in some drivers (like USB)
} Ops;

typedef struct pString {
        char* data;
        Ops actions;
} String;

int f_length(String *self) {
    return strlen(self->data);
}

String *constructString(int n) {
    String *str = malloc(sizeof(String));
    str->data = malloc(sizeof(char) * n);
    str->actions.len = f_length;

    str->data[0] = 0;

    return str;
}

int main() {
    String *p = constructString(BUF_SZ);
    strcpy(p->data, "Hello");
    printf("\n%d", p->actions.len(p));
    return 0;
}
