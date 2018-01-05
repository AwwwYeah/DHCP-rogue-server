#include "../headers/dhcp_messages.h"

u_char *init_opt_offer(u_int srv_ip, struct user_opt *u_opt, u_int offer_addr)
{
	static u_char opt_of[52];

    extern u_int lease_time;       /* ip address lease time */
    extern u_int renewal_time;     /* ip address renewal time */
    extern u_int rebinding_time;   /* ip address rebinding time */
    extern u_int sub_mask;         /* subnet mask */
    extern u_int broad_addr;       /* broadcast address */

    lease_time = htonl(u_opt->lease_time);         /* lease time is 6 hours */
	renewal_time = htonl(u_opt->renewal_time);     /* renewal time is 6 hours */
	rebinding_time = htonl(u_opt->rebinding_time); /* rebinding time is 10 hours and 30 minutes */ 
	sub_mask = inet_addr("255.255.255.0");         /* subnet mask */
	broad_addr = inet_addr("255.255.255.255");     /* broadcast address */

    opt_of[0] = DHCP_OPT_TYPE;
	opt_of[1] = 0x01;
	opt_of[2] = DHCP_OFFER;
	opt_of[3] = DHCP_OPT_SRV_IDENT;
	opt_of[4] = 0x04;
	memcpy(&opt_of[5], &srv_ip, IP_ALEN);
	opt_of[9] = DHCP_OPT_ADDR_LS_TIME;
	opt_of[10] = 0x04;
	memcpy(&opt_of[11], &lease_time, 4);
	opt_of[15] = DHCP_OPT_ADDR_RN_TIME;
	opt_of[16] = 0x04;
	memcpy(&opt_of[17], &renewal_time, 4);
	opt_of[21] = DHCP_OPT_ADDR_RB_TIME;
	opt_of[22] = 0x04;
	memcpy(&opt_of[23], &rebinding_time, 4);
	opt_of[27] = DHCP_OPT_SUB_MASK;
	opt_of[28] = 0x04;
	memcpy(&opt_of[29], &sub_mask, IP_ALEN);
	opt_of[33] = DHCP_OPT_BROAD_ADDR;
	opt_of[34] = 0x04;
	memcpy(&opt_of[35], &broad_addr, IP_ALEN); 
	opt_of[39] = DHCP_OPT_ROUTER;
	opt_of[40] = 0x04;
	memcpy(&opt_of[41], u_opt->router_ip, IP_ALEN);
	opt_of[45] = DHCP_OPT_DNS;
	opt_of[46] = 0x04;
	memcpy(&opt_of[47], u_opt->dns_ip, IP_ALEN);
	opt_of[51] = DHCP_OPT_END;

	return opt_of;
}

u_char *init_opt_ack(u_int srv_ip, struct user_opt *u_opt)
{
	static u_char opt_ack[52];

	lease_time = htonl(u_opt->lease_time); 
	renewal_time = htonl(u_opt->renewal_time);
	rebinding_time = htonl(u_opt->rebinding_time);
	sub_mask = inet_addr("255.255.255.0");
	broad_addr = inet_addr("255.255.255.255");

	opt_ack[0] = DHCP_OPT_TYPE;
	opt_ack[1] = 0x01;
	opt_ack[2] = DHCP_ACK;
	opt_ack[3] = DHCP_OPT_SRV_IDENT;
	opt_ack[4] = 0x04;
	memcpy(&opt_ack[5], &srv_ip, IP_ALEN);
	opt_ack[9] = DHCP_OPT_ADDR_LS_TIME;
	opt_ack[10] = 0x04;
	memcpy(&opt_ack[11], &lease_time, 4);
	opt_ack[15] = DHCP_OPT_ADDR_RN_TIME;
	opt_ack[16] = 0x04;
	memcpy(&opt_ack[17], &renewal_time, 4);
	opt_ack[21] = DHCP_OPT_ADDR_RB_TIME;
	opt_ack[22] = 0x04;
	memcpy(&opt_ack[23], &rebinding_time, 4);
	opt_ack[27] = DHCP_OPT_SUB_MASK;
	opt_ack[28] = 0x04;
	memcpy(&opt_ack[29], &sub_mask, IP_ALEN);
	opt_ack[33] = DHCP_OPT_BROAD_ADDR;
	opt_ack[34] = 0x04;
	memcpy(&opt_ack[35], &broad_addr, IP_ALEN); 
	opt_ack[39] = DHCP_OPT_ROUTER;
	opt_ack[40] = 0x04;
	memcpy(&opt_ack[41], u_opt->router_ip, IP_ALEN);
	opt_ack[45] = DHCP_OPT_DNS;
	opt_ack[46] = 0x04;
	memcpy(&opt_ack[47], u_opt->dns_ip, IP_ALEN);
	opt_ack[51] = DHCP_OPT_END;

	return opt_ack;
}

u_char *init_opt_nak(u_int srv_ip, u_char *msg, u_short msg_len)
{
	static u_char opt_nak[512];

    opt_nak[0] = DHCP_OPT_TYPE;
    opt_nak[1] = 0x01;
    opt_nak[2] = DHCP_NAK;
    opt_nak[3] = DHCP_OPT_SRV_IDENT;
    opt_nak[4] = 0x04;
    memcpy(&opt_nak[5], &srv_ip, IP_ALEN);
    opt_nak[9] = DHCP_OPT_MSG;
    opt_nak[10] = msg_len;
    memcpy(&opt_nak[11], msg, msg_len);
    opt_nak[11 + msg_len] = DHCP_OPT_END;

    return opt_nak;
}