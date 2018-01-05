#ifndef __PACKET_LAYERS_H__
#define __PACKET_LAYERS_H__

#include <string.h>
#include <linux/if_ether.h> 
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include "../proto_struct/dhcp.h" 
#include "layers_sizes.h"
#include "../headers/checksums.h"

struct ethhdr *eth_layer(const u_char *src_mac, const u_char *dst_mac, const u_short proto);


struct iphdr *ip_layer(u_char ihl, u_char version, u_char tos, u_short tot_len,
	                   u_short id, u_short frag_off, u_char ttl, u_char protocol, 
	                   u_int saddr, u_int daddr);

struct udphdr *udp_layer(u_short sport, u_short dport, u_short len, u_short sum);


struct bootphdr *bootp_layer(u_char type, u_char htype, u_char hlen, u_char hops,
	                         u_int xid, u_short secs, u_short flags, u_int ciaddr,
	                         u_int yaddr, u_int siaddr, u_int giaddr, u_char *chaddr,
	                         u_char *sname, u_char *file, u_int dhcp_cookie);

#endif