#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QNAME "/example"

#define SZ 4096

int main() {
        mqd_t server;
        unsigned prio  = 1;

        struct mq_attr attr;

        attr.mq_flags = 0;
        attr.mq_msgsize = SZ;
        attr.mq_curmsgs = 0;

        if ((server = mq_open(QNAME, O_RDONLY | O_CREAT, 0666, &attr)) == -1) {
                perror("Server: mq open failed");
                return 1;
        }
        char buf[SZ];


        if (mq_receive(server, buf, SZ, NULL) == -1) {
                perror ("Server: mq receive error");
                return 1;
        }
        printf("%s", buf);

        return 0;
}
