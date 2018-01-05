install: dhcp_server.o opt_parser.o socket.o interface.o dhcp_messages.o packet_layers.o checksums.o init_opt.o
	gcc -g -o dhcpserver *.o -pthread
	${MAKE} clean

dhcp_server.o: dhcp_server.c
	gcc -c dhcp_server.c

opt_parser.o: opt_parser.c
	gcc -c opt_parser.c

socket.o: functions/socket.c
	gcc -c functions/socket.c

interface.o: functions/interface.c
	gcc -c functions/interface.c

dhcp_messages.o: functions/dhcp_messages.c
	gcc -c functions/dhcp_messages.c

packet_layers.o: functions/packet_layers.c
	gcc -c functions/packet_layers.c

checksums.o: functions/checksums.c
	gcc -c functions/checksums.c

init_opt.o: functions/init_opt.c
	gcc -c functions/init_opt.c

clean:
	rm -f *.o


