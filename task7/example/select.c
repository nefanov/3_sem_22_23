#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	fd_set rfds;
	int ret;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	int num = 1;
	ret = select(num, &rfds, NULL, NULL, &tv);
	if (ret > 0) {
		printf("Data can be achieved\n");
		printf("FD_ISSET: %d\n", FD_ISSET(0, &rfds));
		if (FD_ISSET(0, &rfds)) {
			char buf[4096];
			scanf("%s", buf);
			printf("%s", buf);
		}
		ret = select(num, &rfds, NULL, NULL, &tv);
		if (FD_ISSET(0, &rfds)) {
			printf("Modified\n");
			char buf[4096];
                        scanf("%s", buf);
                        printf("%s", buf);
		}
	} else if (ret == -1) {
		perror("select err");
	} else {
		printf("Timeout!\n");
	}
	return 0;
}
