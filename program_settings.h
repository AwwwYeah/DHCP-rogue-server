#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

/* user settings */

struct user_opt
{
	u_char if_name[24];  /* interface name */
	u_char start_ip[4];  /* start ip that will be issued */
	u_char router_ip[4]; /* router ip */
	u_char dns_ip[4];    /* dns-server ip */
	int lease_time;      /* ip lease time */
	int renewal_time;    /* ip renewal time */
	int rebinding_time;  /* ip rebinding time */
};

struct user_opt *opt_parser(char **main_arg, int arg_count);

void help(void);

#endif
