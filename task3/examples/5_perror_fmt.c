#include <string.h>
#include <stdio.h>
#include <errno.h>

extern int errno;

int main ()
{
  FILE *ptrFile;
  ptrFile = fopen ("fodsfksgjskgjwgjwglwrg.txt","rb");
  if (ptrFile == NULL)
  {
    perror("Case with prefix");
    printf( "Case with fmt string : %s\n", strerror(errno) );
  }
  else
    fclose (ptrFile);
  return 0;
}
