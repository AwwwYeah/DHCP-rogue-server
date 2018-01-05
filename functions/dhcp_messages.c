#include "../headers/dhcp_messages.h"

/* Create offer-message */

u_char *create_offer(u_char *sender_mac, u_char *receiver_mac, u_int sender_ip,
                     struct user_opt *u_opt, u_int offer_ip, u_int id)
	                 
{                   
	static u_char offer[512];      /* memory for offer-message */
    static u_char *opt_of;         /* memory for offer-message options */

    extern u_short ip_tot_len_of;  /* ip total length(headers/dhcp_messages.h) */
    extern u_short udp_tot_len_of; /* udp total length(headers/dhcp_messages.h) */

    struct ethhdr *eth_of;         /* pointer on Ethernet-layer of OFFER-message */
    struct iphdr *ip_of;           /* pointer on IP-layer of OFFER-message */
    struct udphdr *udp_of;         /* pointer on UDP-layer of OFFER-message */
    struct bootphdr *bootp_of;     /* pointer on BOOTP-layer of OFFER-message */

    int i;                         /* Counter for other needs */

	ip_tot_len_of  = IP_HLEN + UDP_HLEN + BOOTP_HLEN + 52;
	udp_tot_len_of = ip_tot_len_of - IP_HLEN;

    /* Creation of layers(headers/packet_layers.h) */
	eth_of = eth_layer(sender_mac, receiver_mac, ETH_P_IP);

	ip_of = ip_layer(5, 4, 0, ip_tot_len_of, 0, 0, 64, IPPROTO_UDP, sender_ip, offer_ip);

	udp_of = udp_layer(67, 68, udp_tot_len_of, 0);

	bootp_of = bootp_layer(BOOTP_REPLY, BOOTP_HTYPE_ETHER, ETH_ALEN, 0, id, 
		                   0, 0, 0, offer_ip, sender_ip, 0, receiver_mac, 
		                   0, 0, DHCP_COOKIE);


    /* Initialisation of OFFER-message options(headers/dhcp_messages.h) */
	opt_of = init_opt_offer(sender_ip, u_opt, offer_ip);

	memcpy(offer, eth_of, ETH_HLEN);
	memcpy(offer + ETH_HLEN, ip_of, IP_HLEN);
	memcpy(offer + ETH_HLEN + IP_HLEN, udp_of, UDP_HLEN);
	memcpy(offer + ETH_HLEN + IP_HLEN + UDP_HLEN, bootp_of, BOOTP_HLEN);
	memcpy(offer + ETH_HLEN + IP_HLEN + UDP_HLEN + BOOTP_HLEN, opt_of, 52);

	printf("[SERVER] OFFER\n");
	printf("Inforamtion about message:\n");
    printf("Client MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
    	   bootp_of->chaddr[0], bootp_of->chaddr[1],
    	   bootp_of->chaddr[2], bootp_of->chaddr[3],
    	   bootp_of->chaddr[4], bootp_of->chaddr[5]);
    printf("Issued IP:  %s\n", inet_ntoa(*(struct in_addr *)&bootp_of->yaddr));
    printf("Session ID: 0x%04x\n\n", ntohl(bootp_of->xid));

	return offer;
}

u_char *create_ack(u_char *sender_mac, u_char *receiver_mac, u_int sender_ip, 
	               struct user_opt *u_opt, u_int ack_ip, u_int id)
{
	static u_char ack[512];
    static u_char *opt_ack;

    struct ethhdr *eth_ack;
    struct iphdr *ip_ack;
    struct udphdr *udp_ack;
    struct bootphdr *bootp_ack;

    extern u_short ip_tot_len_ack;
    extern u_short udp_tot_len_ack;

    int i;

	ip_tot_len_ack  = IP_HLEN + UDP_HLEN + BOOTP_HLEN + 52;
	udp_tot_len_ack = ip_tot_len_ack - IP_HLEN;

	eth_ack = eth_layer(sender_mac, receiver_mac, ETH_P_IP);

	ip_ack = ip_layer(5, 4, 0, ip_tot_len_ack, 0, 0, 64, IPPROTO_UDP, sender_ip, ack_ip);

	udp_ack = udp_layer(67, 68, udp_tot_len_ack, 0);

	bootp_ack = bootp_layer(BOOTP_REPLY, BOOTP_HTYPE_ETHER, ETH_ALEN, 0, id, 
		                   0, 0, 0, ack_ip, sender_ip, 0, receiver_mac, 0, 0, DHCP_COOKIE);

    opt_ack = init_opt_ack(sender_ip, u_opt);

	memcpy(ack, eth_ack, ETH_HLEN);
	memcpy(ack + ETH_HLEN, ip_ack, IP_HLEN);
	memcpy(ack + ETH_HLEN + IP_HLEN, udp_ack, UDP_HLEN);
	memcpy(ack + ETH_HLEN + IP_HLEN + UDP_HLEN, bootp_ack, BOOTP_HLEN);
	memcpy(ack + ETH_HLEN + IP_HLEN + UDP_HLEN + BOOTP_HLEN, opt_ack, 52);

	printf("[SERVER] ACK\n");
	printf("Inforamtion about message:\n");
    printf("Client MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
    	   bootp_ack->chaddr[0], bootp_ack->chaddr[1],
    	   bootp_ack->chaddr[2], bootp_ack->chaddr[3],
    	   bootp_ack->chaddr[4], bootp_ack->chaddr[5]);
    printf("Issued IP:  %s\n", inet_ntoa(*(struct in_addr *)&bootp_ack->yaddr));
    printf("Session ID: 0x%04x\n\n", ntohl(bootp_ack->xid));

	return ack;
}

u_char *create_nak(u_char *sender_mac, u_char *receiver_mac, u_int sender_ip, u_int id, u_char *msg, u_short msg_len)
{
    int i;

    static u_char nak[512];
    static u_char *opt_nak;

    struct ethhdr *eth_nak;
    struct iphdr  *ip_nak;
    struct udphdr *udp_nak;
    struct bootphdr *bootp_nak;

    extern u_short ip_tot_len_nak;   /* will be later */
    extern u_short udp_tot_len_nak;

    ip_tot_len_nak = IP_HLEN + UDP_HLEN + BOOTP_HLEN + 12 + msg_len;
    udp_tot_len_nak = ip_tot_len_nak - IP_HLEN;

    eth_nak = eth_layer(sender_mac, receiver_mac, ETH_P_IP);

    ip_nak = ip_layer(5, 4, 0, ip_tot_len_nak, 0, 0, 64, IPPROTO_UDP, sender_ip, inet_addr("255.255.255.255"));

    udp_nak = udp_layer(67, 68, udp_tot_len_nak, 0);

    bootp_nak = bootp_layer(BOOTP_REPLY, BOOTP_HTYPE_ETHER, ETH_ALEN, 0, id, 0, 0, 0, 0, 0, 0,
    	                    receiver_mac, 0, 0, DHCP_COOKIE);

    opt_nak = init_opt_nak(sender_ip, msg, msg_len);

    memcpy(nak, eth_nak, ETH_HLEN);
    memcpy(nak + ETH_HLEN, ip_nak, IP_HLEN);
    memcpy(nak + ETH_HLEN + IP_HLEN, udp_nak, UDP_HLEN);
    memcpy(nak + ETH_HLEN + IP_HLEN + UDP_HLEN, bootp_nak, BOOTP_HLEN);
    memcpy(nak + ETH_HLEN + IP_HLEN + UDP_HLEN + BOOTP_HLEN, opt_nak, 12 + msg_len);

    printf("[SERVER] NAK to: %02x", bootp_nak->chaddr[0]);
    for(i = 1; i < ETH_ALEN; i++)
    	printf(":%02x", bootp_nak->chaddr[i]);

    printf("|| id: 0x%04x msg: %s\n", ntohl(bootp_nak->xid), msg);

    return nak;
}