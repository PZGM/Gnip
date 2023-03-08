#include "ft_ping.h"

t_ggnip ggnip;

void intHandler(int dummy) {
	(void)dummy;
	print_stats(ggnip.host, ggnip.stats);
	free(ggnip.res);
	free(ggnip.ip_str);
	exit(0);
}

int main(int argc, char **argv) {
  
  	if (argc < 2)
	{
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

	ggnip.ip_str = get_addr(ggnip.host, &(ggnip.res));
	memset(&dest_addr, 0, sizeof(dest_addr));

	int sockfd = socketfd(ggnip.res, &dest_addr);

	int seq = 0;
	gettimeofday(&start_time, NULL);
	ggnip.stats.received = 0;
	ggnip.stats.sent = 0;
	ggnip.stats.start_time = start_time;
	ggnip.stats.rtt = NULL;
	ggnip.stats.total_time_ms = 0;

	parse(argc, argv, host_pos);
	printf("PING %s(%s) %i(%i) data bytes\n", ggnip.host, ggnip.ip_str, ICMP_PAYLOAD_SIZE, PACKET_SIZE);

	while (42) {
		seq++;

		ggnip.stats.sent += send_ping(sockfd, &dest_addr,seq, &start_time);
		double ping = recv_ping(sockfd, &dest_addr, seq, &start_time, ggnip.host, ggnip.ip_str);
		update_stats(&(ggnip.stats), ping);
		sleep(1);
	}
	
	return 0;
}
