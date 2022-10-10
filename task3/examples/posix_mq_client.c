#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SZ 4096

#define QNAME "/example"

int main() {
        mqd_t queue;
        struct mq_attr attr;
        unsigned prio = 1;
        attr.mq_flags = 0;
        attr.mq_msgsize = SZ;
        attr.mq_curmsgs = 0;

        if ((queue = mq_open(QNAME, O_WRONLY, 0666, &attr)) == -1) {
                perror("Client: mq open error");
                return 1;
        }

        char buf[4096];
        strcpy(buf, "hello world");

        if (mq_send(queue, buf, strlen(buf)+1, prio) == -1) {
                perror("Client: mq send error");
                return 1;
        }

        return 0;
}
