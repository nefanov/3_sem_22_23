#include "msq.h"




int main()
{
	int letter = 1;
	char name[] = "main1.c";


	int msqid = get_msqid(name);


	while (letter != 0)
	{

		letter = read(msqid);

	}


	return 0;

}