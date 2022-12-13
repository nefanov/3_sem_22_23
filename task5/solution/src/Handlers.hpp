#ifndef _H_HANDLERS_
#define _H_HANDLERS_

#include <signal.h>
#include <stdio.h>
#include <assert.h>
 
void ParentHandler(int signo, siginfo_t *siginf, void *context);

void ChildHandler(int signo, siginfo_t *siginf, void *context);

void EndHandler(int signo);

#endif