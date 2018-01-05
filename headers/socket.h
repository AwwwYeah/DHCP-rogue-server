#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "../headers/interface.h"
#include <netpacket/packet.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int create_sock(const struct if_data *md, u_short protocol);

#endif