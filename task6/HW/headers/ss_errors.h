#pragma once

#include "ss_log.h"

struct MapsLine;
typedef struct MapsLine MapsLine;

int check_parse_maps_err(int count);
int check_parse_maps_line_err(MapsLine* PML);