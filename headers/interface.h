#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <linux/if_ether.h>
#include "layers_sizes.h"

struct if_data
{
    u_char if_mac[ETH_ALEN];
    u_char if_ip[IP_ALEN];
    u_int if_index;
};

/* initialization of interface */
u_char *init_interface(void);

/* get information about interface */
struct if_data *get_if_data(const u_char *if_name);

#endif