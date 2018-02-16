# DHCP-rogue-server

One of first steps of MITM attacks. Server for sending your DHCP answers to clients in a local network. 
If original server is active, this program will be useless. You can neutralize original DHCP server with DHCP flooding 
or DHCP starvation attacks.

*Compilation*

            make install
            
*Options*

            General settings:
            -i - Interface
            -h (--help) - Help note
            
            Addresses settings:
            -s (--start-ip) - Server will give addresses beginning with argument
            -r (--router)   - Router address in sent OFFER or ACK
            -d (--dns)      - DNS server address in sent OFFER or ACK
            
            Time settings:
            -lt (--lease-time)     - Address lease time in seconds
            -rn (--renewal-time)   - Address renewal time in seconds
            -rb (--rebinding-time) - Address rebinding time in seconds
            
*Using*

            ./dhcpserver -i wlan0 --start-ip 192.168.100.2 --lease-time 12000 --renewal-time 6000 --rebinding-time 10000
            
