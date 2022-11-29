#include "../headers/ss_errors.h"

int check_parse_maps_err(int count) {
  if (count == -1) {
    // Parsing maps' lines fail
    create_log("Parsing maps file: FAIL");
    return -1;
  } else {
    create_log("Parsing maps file is completed. "
               "There are %d lines\n", count);
  }
  return 0;
}
int check_parse_maps_line_err(MapsLine* PML) {
  // TODO: implement check error
  return 0;
}