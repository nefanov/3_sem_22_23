#include "message_queue.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    exit(1);
  }
  msq_snd(argv[1]);
  return 0;
}