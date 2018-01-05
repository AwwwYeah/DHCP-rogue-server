#include "../headers/checksums.h"

u_short checksum(u_short *addr, int len)
{
	int sum = 0;
	u_short answer = 0;
	u_short *w = addr;
	int nleft = len;

	while(nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}

	if(nleft == 1)
	{
		*(u_char *)(&answer) = *(u_char *)w;
		sum += answer;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;

	return answer;
}