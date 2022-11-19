#include "shared_memory.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    exit(1);
  }
  shared_mem_rcv(argv[1]);
  return 0;
}