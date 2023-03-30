#ifndef _H_HANDLERS_
#define _H_HANDLERS_

#include "Config.hpp"

int RegHandle(int cmdFd, int ackFd, int* cmdSet, int* fileSet, int plc, struct pollfd* fds);

#endif