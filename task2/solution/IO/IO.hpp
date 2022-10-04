#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdio.h>

void Input(FILE *srcFP, char *dstBuf, size_t size);

void Output(FILE *dstFP, char *srcBuf, size_t size);

#endif