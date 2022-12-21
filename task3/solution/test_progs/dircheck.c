#include <dirent.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int read_directory(const char* nm) {
  struct dirent *dir_entry;
  DIR* dir = opendir(nm);
  assert(dir != NULL);
  printf("Content:\n");
  while ((dir_entry = readdir(dir)) != NULL) {
      if (dir_entry->d_type == DT_REG) {
        printf("\t%s\n", dir_entry->d_name);
      } else if (dir_entry->d_type == DT_DIR && strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..")) {
        read_directory(strcat(strcat(nm,"/"), dir_entry->d_name));
      }
  }
  closedir(dir);
  return 0;
}

int main() {
  struct dirent *dir_entry;
  DIR* dir = opendir(".");
  assert(dir != NULL);
  printf("Content:\n");
  while ((dir_entry = readdir(dir)) != NULL) {
      if (dir_entry->d_type == DT_REG) {
        printf("\t%s\n", dir_entry->d_name);
      } else if (dir_entry->d_type == DT_DIR && strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..")) {
        //recursve call
      }
  }
  closedir(dir);
  return 0;
}