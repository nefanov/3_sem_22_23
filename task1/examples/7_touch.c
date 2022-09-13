#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/* this program implements the logic of 'touch' utilite: it creates files on fs */

int main(int argc, char **argv)
{
	int perm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	int i;
	for (i = 1; i < argc; ++i) {
		int fd=open(argv[i], O_WRONLY | O_CREAT, perm);

		if (fd < 0) {
			fprintf(stderr, "Fauled to create %s\n", argv[i]);

			return -1;
		}
		close(fd);

	}

	return 0;
}
