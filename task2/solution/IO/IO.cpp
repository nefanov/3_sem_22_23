#include "IO.hpp"

void Input(FILE *srcFP, char *dstBuf, size_t size) {
    fread(dstBuf, sizeof(char), size, srcFP);
}

void Output(FILE *dstFP, char *srcBuf, size_t size) {
    fwrite(srcBuf, sizeof(char), size, dstFP);
}