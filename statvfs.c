#include <stdio.h>
#include <stdlib.h>
#include <sys/statvfs.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <device>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct statvfs fsdata;
    int result = statvfs(argv[1], &fsdata);
    if (result != 0) {
        fprintf(stderr, "Failed to stat: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    printf("Disk %s:\n", argv[1]);
    printf("  file system block size: %lu\n", fsdata.f_bsize);
    printf("  fragment size: %lu\n", fsdata.f_frsize);
    printf("  size of fs in f_frsize units: %d\n", fsdata.f_blocks);
    printf("  # free blocks: %d\n", fsdata.f_bfree);
    printf("  # free blocks for unprivileged users: %d\n", fsdata.f_bavail);
    printf("  # inodes: %d\n", fsdata.f_files);
    printf("  # free inodes: %d\n", fsdata.f_ffree);
    printf("  # free inodes for unprivileged users: %d\n", fsdata.f_favail);
    printf("  file system ID: %lu\n", fsdata.f_fsid);
    printf("  mount flags: %lu\n", fsdata.f_flag);
    printf("  maximum filename length: %lu\n", fsdata.f_namemax);

    return EXIT_SUCCESS;
}