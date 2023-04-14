#include "ft_ping.h"

t_ggnip ggnip;

void intHandler(int dummy) {
	(void)dummy;
	print_stats(ggnip.host, ggnip.stats);
	free(ggnip.res);
	free(ggnip.ip_str);
	exit(0);
}

void alarmHandler(int dummy) {
	(void)dummy;
	alarm(1);
	ggnip.stats.sent += send_ping(ggnip.sockfd, (ggnip.dest_addr),ggnip.seq);
	ggnip.seq++;
}

int main(int argc, char **argv) {
  
  	if (argc < 2)
	{
		write(2, "ft_ping: usage error: Destination address required\n", 51);
        exit(1);
    }
	
	int x = 0;
	int host_pos = -1;
	while (++x < argc)
	{
		if (argv[x][0] != '-' || !argv[x][1])
		{
			ggnip.host = argv[x];
			host_pos = x;
		}
	}
	signal(SIGINT, intHandler);
	struct sockaddr_in dest_addr;
	struct timeval start_time;

	parse(argc, argv, host_pos);

	ggnip.ip_str = get_addr(ggnip.host, &(ggnip.res), host_pos);
	memset(&dest_addr, 0, sizeof(dest_addr));

	int sockfd = 0;
	if (host_pos >= 0)
		sockfd = socketfd(ggnip.res, &dest_addr);

	gettimeofday(&start_time, NULL);
	ggnip.stats.received = 0;
	ggnip.stats.sent = 0;
	ggnip.stats.start_time = start_time;
	ggnip.stats.rtt = NULL;
	ggnip.stats.total_time_ms = 0;

	printf("PING %s(%s) %i(%i) data bytes\n", ggnip.host, ggnip.ip_str, ICMP_PAYLOAD_SIZE, PACKET_SIZE);

	signal(SIGALRM, alarmHandler);
	alarm(1);
	ggnip.dest_addr = &dest_addr;
	ggnip.sockfd = sockfd;
	ggnip.seq = 0;

	while (42) {
		double ping = recv_ping(sockfd, &dest_addr, ggnip.host, ggnip.ip_str);
		
		if (ping > 0)
			update_stats(&(ggnip.stats), ping);
	}
	
	return 0;
}
