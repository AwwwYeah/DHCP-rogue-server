#ifndef __CHECKSUM_H_
#define __CHECKSUM_H_

#include <sys/types.h>

u_short checksum(u_short *addr, int len);

#endif