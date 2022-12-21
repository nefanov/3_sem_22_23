#include <dirent.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/statvfs.h>

int main(int argc, char* argv[]) {
  char* path = argv[1];
  struct statvfs fsdata;
  int res = statvfs(path, &fsdata);
  if (res != 0) {
    printf("Error\n");
  } else {
    printf("Path: %s\n", path);
    printf("FS block size: %lu\n", fsdata.f_bsize);
    printf("FS flag: %lu\n", fsdata.f_flag);
    printf("inodes number: %lu\n", fsdata.f_files);

  }
  
  return 0;
}