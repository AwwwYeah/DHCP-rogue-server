#include "program_settings.h"

struct user_opt *opt_parser(char **main_arg, int arg_count)
{
	short i, a, b;
	static struct user_opt u_opt;

	memset(&u_opt, 0x0, sizeof(struct user_opt));

	for(i = 1; i < arg_count; i++)
	{
		if(!strcmp("-i", main_arg[i]))
		{
			i++;

			if(main_arg[i] == 0)
			{
				printf("[ERROR] Interface name isn't set\n");

				return NULL;
			}

			strncpy(u_opt.if_name, main_arg[i], 24);

			continue;
		}

		if(!strcmp("--start-ip", main_arg[i]) || !strcmp("-s", main_arg[i]))
		{
			i++;

			if(main_arg[i] == 0)
			{
				printf("[ERROR] Start ip isn't set\n");

				return NULL;
			}

			*(u_int *)u_opt.start_ip = inet_addr(main_arg[i]);

			continue;
		}

		if(!strcmp("--router", main_arg[i]) || !strcmp("-r", main_arg[i]))
		{
			i++;

			if(main_arg[i] == 0)
			{
				printf("[ERROR] Router IP isn't set\n");

				return NULL;
			}

			*(u_int *)u_opt.router_ip = inet_addr(main_arg[i]);

			continue;
		}

		if(!strcmp("--dns", main_arg[i]) || !strcmp("-d", main_arg[i]))
		{
			i++;

			if(main_arg[i] == 0)
			{
				printf("[ERROR] DNS server IP isn't set\n");

				return NULL;
			}

			*(u_int *)u_opt.dns_ip = inet_addr(main_arg[i]);

			continue;
		}

		if(!strcmp("--lease-time", main_arg[i]) || !strcmp("-lt", main_arg[i]))
		{
			i++;

			if(main_arg[i] == 0)
			{
				printf("[ERROR] Lease time isn't set\n");

				return NULL;
			}

			u_opt.lease_time = atoi(main_arg[i]);

			continue;
		}

		if(!strcmp("--renewal-time", main_arg[i]) || !strcmp("-rt", main_arg[i])) 
		{
			i++;

			if(main_arg[i] == 0)
			{
				printf("[ERROR] Renewal time isn't set\n");

				return NULL;
			}

			u_opt.renewal_time = atoi(main_arg[i]);

			continue;
		}

		if(!strcmp("--rebinding-time", main_arg[i]) || !strcmp("-rbt", main_arg[i]))
		{
			i++;

			if(main_arg[i] == 0)
			{
				printf("[ERROR] Rebinding time isn't set\n");

				return NULL;
			}

			u_opt.rebinding_time = atoi(main_arg[i]);

			continue;
		}

		if(!strcmp("-h", main_arg[i]) || !strcmp("--help", main_arg[i]) )
		{
			help();

			return NULL;
		}

		printf("[ERROR] Unknown argument \"%s\"\n", main_arg[i]);

		return NULL;
	}

	return &u_opt;
}

void help(void)
{
	printf("\n*****DHCP server by AwwwYeah******\n\n");
	
	printf("-i            - Interface\n");
	printf("-h (--help)   - Show this note and finish the program\n\n");

    	printf("Addresses settings:\n");
	printf("-s (--start-ip)   - Server will give addresses beginning with argument\n");
	printf("-r (--router)     - Router's ip address\n");
	printf("-d (--dns)        - DNS server's ip address\n\n");

	printf("Time settings:\n");
	printf("-lt (--lease-time)     - Address lease time(seconds)\n");
	printf("-rt (--renewal-time)   - Address renewal time(seconds)\n");
	printf("-rbt(--rebinding-time) - Address rebinding time(seconds)\n\n");
}
