#include "../headers/packet_layers.h"

struct ethhdr *eth_layer(const u_char *src_mac, const u_char *dst_mac, const u_short proto)
{
	static struct ethhdr eth;

	memset(&eth, 0x0, ETH_HLEN);

	if(src_mac != NULL)
		memcpy(eth.h_source, src_mac, ETH_ALEN);

	if(dst_mac != NULL)
		memcpy(eth.h_dest, dst_mac, ETH_ALEN);
		
	if(proto != 0)
		eth.h_proto = htons(proto);

	return &eth;
}

struct iphdr *ip_layer(u_char ihl, u_char version, u_char tos, u_short tot_len,
	                   u_short id, u_short frag_off, u_char ttl, u_char protocol, 
	                   u_int saddr, u_int daddr)
{
	static struct iphdr ip;

    ip.ihl      = ihl;
    ip.version  = version;
    ip.tos      = tos;
    ip.tot_len  = htons(tot_len);
    ip.id       = htons(id);
    ip.frag_off = htons(0);
    ip.ttl      = ttl;
    ip.protocol = protocol;
    ip.check    = 0;
    ip.saddr    = saddr;
    ip.daddr    = daddr;

    ip.check = checksum((u_short *)&ip, ip.ihl*4);

    return &ip;
}

struct udphdr *udp_layer(u_short sport, u_short dport, u_short len, u_short sum)
{
	static struct udphdr udp;

	udp.uh_sport = htons(sport);
	udp.uh_dport = htons(dport);
	udp.uh_ulen  = htons(len);
	udp.uh_sum   = 0;

	return &udp;
}

struct bootphdr *bootp_layer(u_char type, u_char htype, u_char hlen, u_char hops,
	                         u_int xid, u_short secs, u_short flags, u_int ciaddr,
	                         u_int yaddr, u_int siaddr, u_int giaddr, u_char *chaddr,
	                         u_char *sname, u_char *file, u_int dhcp_cookie)
{
    static struct bootphdr bootp;

    bootp.type  = type;
    bootp.htype = htype;
    bootp.hlen  = hlen;
    bootp.hops  = hops;
    bootp.xid   = htonl(xid);
    bootp.secs  = htons(secs);
    bootp.flags = htons(flags);
    bootp.ciaddr = ciaddr;
    bootp.yaddr  = yaddr;
    bootp.siaddr = siaddr;
    bootp.giaddr = giaddr;

    if(sname == NULL)
    	memset(bootp.sname, 0x0, 64);

    else
    	memcpy(bootp.sname, sname, 64);

    if(file == NULL)
    	memset(bootp.file, 0x0, 128);

    else
    	memcpy(bootp.file, file, 128);

    memcpy(bootp.chaddr, chaddr, ETH_ALEN);

    bootp.dhcp_cookie = htonl(DHCP_COOKIE);

    return &bootp;
}