#include "CommonFunctions.hpp"

int GetFSize(FILE *f) {
    assert(f);

    struct stat statBuf = {};

    int result = fstat(fileno(f), &statBuf);
    ASSERTED(fstat, result, -1, -1);
    
    int fSize = statBuf.st_size;

    return fSize;
}

int GetBufSize(int argc, char **argv) {
    // if (argc != 4) {
    //     printf("Wrong arguments\n");
    //     return -1;
     if (strcmp(argv[1], "SMALL") == 0)
        return SMALL;
    else if (strcmp(argv[1], "MEDIUM") == 0)
        return MEDIUM;
    else if (strcmp(argv[1], "BIG") == 0)
        return BIG;
    else {
        printf("Wrong arguments\n");
        return -1;
    }
}