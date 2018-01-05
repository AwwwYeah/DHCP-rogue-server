#ifndef __SERVER_DATA_H__
#define __SERVER_DATA_H__

struct current_discover
{
	u_char client_mac[ETH_ALEN];
    u_int client_id;
};

struct current_request
{
	u_char client_mac[ETH_ALEN];
    u_char client_ip[IP_ALEN];
    u_int client_id;
};

u_char current_ip[IP_ALEN];
u_char requested_ip[IP_ALEN];

#endif