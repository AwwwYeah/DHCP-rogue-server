#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include "program_settings.h"
#include "headers/socket.h"
#include "headers/dhcp_messages.h"
#include "headers/server_data.h"

/* General server's function */
void server(void *user_options);

/* Initialization of program settings */
int settings_init(struct user_opt *u_opt);

int GOT_SIGINT = 1;

void sigint_handler(int arg);

pthread_t recv_and_send_messages;

int main(int argc, char **argv)
{
	struct user_opt *u_opt; /* user options */

    /* Parsing of options. Structure with options will be sent in new thread. */
    if((u_opt = opt_parser(argv, argc)) == NULL)
    	return -1;

    struct sigaction act;
    sigset_t set;

    /* Set signal handler */
    memset(&act, 0x0, sizeof(struct sigaction));
    memset(&set, 0x0, sizeof(set));
    act.sa_handler = sigint_handler;
    sigaddset(&set, SIGINT);
    act.sa_mask = set;
    sigaction(SIGINT, &act, 0);

    pthread_create(&recv_and_send_messages, NULL, (void *)&server, (void *)u_opt);

    pthread_join(recv_and_send_messages, NULL);

    printf("[INFO] Finish program...\n\n");

    exit(EXIT_SUCCESS);
}

void server(void *user_options)
{
    int sock;                        /* socket */
    struct if_data *ifd;             /* pointer on structure if_data */

    u_char message[1024];            /* memory for received packet */
    struct ethhdr *eth_msg;          /* pointer on ethernet-layer of received packet */
    struct iphdr  *ip_msg;           /* pointer on ip-layer of received packet */
    struct udphdr *udp_msg;          /* pointer on udp-laer of received packet */
    struct bootphdr *bootp_msg;      /* pointer on bootp-layer of received packet */
    u_char *opt_msg;                 /* pointer on DHCP-options of received packet */
    u_short opt_len;                 /* DHCP options length */
    u_short recv_bytes;              /* Received bytes */

    u_char *offer;                   /* pointer on offer-message(it will be created later) */
    u_char *ack;                     /* pointer on ack-message */
    u_char *nak;                     /* pointer on nak-message */

    struct current_discover cur_dis; /* structure for saving of discover's data(headers/server_data.h) */
    struct current_request cur_req;  /* structure for saving of request's data(headers/server_data.h) */

    struct user_opt *u_opt;          /* pointer on user options(program_settings.h) */

    u_short src_port;                /* source port of received message */
    extern u_char current_ip[IP_ALEN];         /* current address that will be issued to client(program_setting.h) */
    extern u_char requested_ip[IP_ALEN];       /* requested ip address */              

    int i;                           /* counter for other needs */
    char select;                     /* run server or finish */

    /* Initialization of user options(start ip, router ip) */
    u_opt = (struct user_opt *)user_options;
    if(settings_init(u_opt) == -1)
    	pthread_exit(NULL);


    /* Get interface data(MAC, IP, Index) */
    if((ifd = get_if_data(u_opt->if_name)) == NULL)
    {
        printf("[ERROR] get_if_data(): %s(%s)\n", strerror(errno), u_opt->if_name);

        return;
    }

    /* Create a packet socket */
    if((sock = create_sock(ifd, ETH_P_IP)) < 0)
    {
        printf("[ERROR] create_sock() error: %s\n", strerror(errno));

        return;
    }

    /* Get current offer ip */
    memcpy(current_ip, u_opt->start_ip, IP_ALEN);

    printf("[INFO] Interfase is used: %s\n", u_opt->if_name);
    printf("[INFO] My addr:        %s\n", inet_ntoa(*(struct in_addr *)ifd->if_ip));
    printf("[INFO] Router:         %s\n", inet_ntoa(*(struct in_addr *)&u_opt->router_ip));
    printf("[INFO] DNS:            %s\n", inet_ntoa(*(struct in_addr *)&u_opt->dns_ip));
    printf("[INFO] Issued ip from: %s\n", inet_ntoa(*(struct in_addr *)&current_ip));
    printf("[INFO] Lease time:     %d seconds(%d hours %d minutes)\n", u_opt->lease_time, u_opt->lease_time/3600, (u_opt->lease_time / 60) % 60);
    printf("[INFO] Renewal time:   %d seconds(%d hours %d minutes)\n", u_opt->renewal_time, u_opt->renewal_time/3600, (u_opt->renewal_time / 60) % 60);
    printf("[INFO] Rebinding time: %d seconds(%d hours %d minutes)\n\n", u_opt->rebinding_time, u_opt->rebinding_time/3600, (u_opt->rebinding_time / 60) % 60);

    printf("[SERVER] Run server?(ANY key - yes, N(n) - no): ");

    while( select = getchar())
    { 
    	if(select == '\n')
        	continue;

    	if(select == 'y' || select == 'Y')
    	{
    		printf("[SERVER] Server is running\n");
    		printf("[SERVER] Waiting for input packets...\n\n");
    		break;
    	}

    	else if(select == 'n' || select == 'N')
    	{
            printf("[SERVER] Exit...\n");

    	    pthread_exit(NULL);
    	}
 
        printf("y(Y) - yes, n(N) - no: ");
    }

    eth_msg = (struct ethhdr *)message;
    ip_msg  = (struct iphdr *)(message + ETH_HLEN);

    /* set source port for filter */
    src_port = htons(68);

    while(GOT_SIGINT)
    {
        recv_bytes = recv(sock, message, 1024, 0);

        udp_msg = (struct udphdr *)(message + ETH_HLEN + ip_msg->ihl*4);

        /* Received packet's source port == 68 */
        if(udp_msg->uh_sport == src_port)
        {
            bootp_msg = (struct bootphdr *)(message + ETH_HLEN + ip_msg->ihl*4 + UDP_HLEN);

            /* Type of BOOTP-layer is REQUEST: */
            if(bootp_msg->type == BOOTP_REQUEST)
            {
                opt_msg = message + ETH_HLEN + ip_msg->ihl*4 + UDP_HLEN + BOOTP_HLEN;

                opt_len = recv_bytes - (ETH_HLEN + ip_msg->ihl*4 + UDP_HLEN + BOOTP_HLEN);

                /* Type of DHCP-layer is DISCOVER */
                if(opt_msg[0] == DHCP_OPT_TYPE && opt_msg[2] == DHCP_DISCOVER)
                {
                    /* copy client mac to current_discover structure */
                    memcpy(&cur_dis.client_mac, bootp_msg->chaddr, ETH_ALEN);

                    /* copy transaction id to current_discover structure */
                    cur_dis.client_id = bootp_msg->xid;

                    printf("[SERVER] DISCOVER\n");
                    printf("Information about message:\n");
                    printf("__________________________\n");
                    printf("Client MAC:   %02x:%02x:%02x:%02x:%02x:%02x\n", 
                    	   bootp_msg->chaddr[0], bootp_msg->chaddr[1],
                    	   bootp_msg->chaddr[2], bootp_msg->chaddr[3],
                    	   bootp_msg->chaddr[4], bootp_msg->chaddr[5]);
                    printf("Session ID:   0x%04x\n\n", ntohl(bootp_msg->xid));

                    /* Create OFFER message to target host */
                    offer = create_offer(ifd->if_mac, cur_dis.client_mac, *(u_int*)ifd->if_ip, 
                    	                 u_opt, *(u_int *)&current_ip, ntohl(cur_dis.client_id));
                
                    send(sock, offer, OFFER_LEN, 0);

                    if(current_ip[3] == 254)
                    {
                    	current_ip[2]++;

                    	current_ip[3] = 0;
                    }
                    
                    current_ip[3]++;

                    continue;
                }
 
                /* Type of DHCP-message is REQUEST: */
                if( (opt_msg[0] == DHCP_OPT_TYPE) && (opt_msg[2] == DHCP_REQUEST) )
                {

                    /* copy client mac to current_request structure */
                    memcpy(&cur_req.client_mac, bootp_msg->chaddr, ETH_ALEN);

                    /* copy client id to current_request structure */
                    cur_req.client_id = bootp_msg->xid;

                    /* copy requested address */
                    for(i = 0; i < opt_len; i++)
                    	if(opt_msg[i] == DHCP_OPT_REQ_ADDR)
                    	{
                    		i+=2;

                    		memcpy(requested_ip, &opt_msg[i], IP_ALEN);

                    		break;
                    	}

                    printf("[SERVER] REQUEST\n");
                    printf("Information about message:\n");
                    printf("Client MAC:   %02x:%02x:%02x:%02x:%02x:%02x\n", 
                    	   bootp_msg->chaddr[0], bootp_msg->chaddr[1],
                    	   bootp_msg->chaddr[2], bootp_msg->chaddr[3],
                    	   bootp_msg->chaddr[4], bootp_msg->chaddr[5]);
                    printf("Requested IP: %s\n", inet_ntoa(*(struct in_addr *)&requested_ip));
                    printf("Session ID:   0x%04x\n\n", ntohl(bootp_msg->xid));

                    /* Create ACK message to target host */
                	ack = create_ack(ifd->if_mac, cur_req.client_mac, *(u_int *)ifd->if_ip, 
                		             u_opt, *(u_int *)&requested_ip, ntohl(cur_req.client_id));

                    send(sock, ack, ACK_LEN, 0);
                }
            }
        }
    }

    pthread_exit(NULL);
}

int settings_init(struct user_opt *u_opt)
{
	u_char *interface;

	if(u_opt->if_name[0] == 0)
	{
		if((interface = init_interface()) == NULL)
		{
			printf("[ERROR] Can't initialize any interface\n");

			return -1;
		}

		strncpy(u_opt->if_name, interface, strlen(interface));
	}

	struct if_data *ifd; 

	if((ifd = get_if_data(u_opt->if_name)) == NULL)
	{
		printf("[ERROR] get_if_data(): %s(%s)\n",strerror(errno), u_opt->if_name);

		return -1;
	}

    /* If start ip isn't set. Default ip is used*/
    if(u_opt->start_ip[0] == 0)
    {
    	memcpy(&u_opt->start_ip, ifd->if_ip, IP_ALEN);

    	u_opt->start_ip[3] = 2;

    	if(u_opt->start_ip[3] == 2 && ifd->if_ip[3] == 2)
          u_opt->start_ip[3]++;
    }

    /* Router IP isn't set. Your interface IP is used as router IP */
    if(u_opt->router_ip[0] == 0)
    	memcpy(&u_opt->router_ip, ifd->if_ip, IP_ALEN);
    

    /* DNS-server IP isn't set. Yout interface IP is used as DNS-server IP */
    if(u_opt->dns_ip[0] == 0)
    	memcpy(&u_opt->dns_ip, ifd->if_ip, IP_ALEN);

    /* Lease time */
    if(u_opt->lease_time == 0)
       u_opt->lease_time = 21600;

    /* Renewal time */
    if(u_opt->renewal_time == 0)
       u_opt->renewal_time = 21600;

    /* Rebinding time */
    if(u_opt->rebinding_time == 0)
       u_opt->rebinding_time = 37800;

    return 0;
}

void sigint_handler(int sig)
{
	printf("[SERVER] Got signal SIGINT\n");

    GOT_SIGINT--;

    return;
}