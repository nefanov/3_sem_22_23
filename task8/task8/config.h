#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>

#define MAX_LINE 1024
#define ADDR_LINE 256
#define SRV_MAX 4
#define PORT1 7000
#define PORT2 7001
#define PORT3 7002
#define PORT4 7003
#define DEFAULT_ADDR INADDR_ANY
#define CLT_DEFAULT "127.0.0.1"
#define MAX_CLT 64
#define Y_MIN 0
#define Y_MAX 1000
#define ACCURACY 10000000 