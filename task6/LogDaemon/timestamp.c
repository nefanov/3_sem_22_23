#include <time.h>

// Put timestamp in arg string
void get_timestamp (char* timestamp)
{
  time_t rawtime;
  struct tm * timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  sprintf (timestamp,"%s",asctime (timeinfo));
  puts(timestamp);

  for (; *timestamp; timestamp++)
    if (*timestamp == ' ' || *timestamp == '\n') *timestamp = '_';

  return 0;
}