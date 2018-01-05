#ifndef __DHCP_MESSAGES_H__
#define __DCHP_MESSAGES_H__

#include <sys/types.h>
#include "../headers/packet_layers.h"
#include "../program_settings.h"

#define OFFER_LEN 334
#define ACK_LEN 334

static u_short ip_tot_len_of;   /* ip total length of offer message */
static u_short udp_tot_len_of;  /* udp total length of offer message */

static u_short ip_tot_len_ack;  /* ip total length of ack message */
static u_short udp_tot_len_ack; /* udp total length of ack message */

static u_short ip_tot_len_nak;  /* ip total length of nak message */
static u_short udp_tot_len_nak; /* udp total length of nak message */

static u_short msg_len;         /* message len(option 56) */

static u_int lease_time;        /* ip address lease time */
static u_int renewal_time;      /* ip address renewal time */
static u_int rebinding_time;    /* ip address rebinding time */
static u_int sub_mask;          /* subnet mask */
static u_int broad_addr;        /* broadcast address */

/* Functions of messages creating */

/* 1. Creating of offer message */
u_char *create_offer(u_char *sender_mac, u_char *receiver_mac, u_int sender_ip, 
	                        struct user_opt *u_opt, u_int offer_ip, u_int id);

/* 2. Creating of ack message */
u_char *create_ack(u_char *sender_mac, u_char *receiver_mac, u_int sender_ip, 
	                      struct user_opt *u_opt, u_int ack_ip, u_int id);

/* 3. Creating of nak message */

u_char *create_nak(u_char *sender_mac, u_char *receiver_mac, u_int sender_ip, u_int id, u_char *msg, u_short msg_len);

/* Functions for initialization of message's options */

/* 1. Initialization of offer messages's options */
u_char *init_opt_offer(u_int srv_ip, struct user_opt *u_opt, u_int offer_addr);

/* 2. Initiallization of ack message's options */
u_char *init_opt_ack(u_int srv_ip, struct user_opt *u_opt);

/* 3. Initialization of nak message's options */
u_char *init_opt_nak(u_int srv_ip, u_char *msg, u_short msg_len);

#endif