#include "head.hpp"


int main()
{
	int letter = 1;
	char name[] = "main_send.cpp";

	int msqid = get_msqid(name);

	while (letter != 0)
	{

		letter = send(msqid);

	}


	return 0;
}