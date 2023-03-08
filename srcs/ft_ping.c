#include "ft_ping.h"

t_stats stats;

char *host;
struct addrinfo *res;
char * ip_str;

void intHandler(int dummy) {
	(void)dummy;
	print_stats(host, stats);
	free(res);
	free(ip_str);
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
			host = argv[x];
			host_pos = x;
		}
	}
	signal(SIGINT, intHandler);
	struct sockaddr_in dest_addr;
	struct timeval start_time;

	ip_str = get_addr(host, &res);
	memset(&dest_addr, 0, sizeof(dest_addr));

	int sockfd = socketfd(res, &dest_addr);

	int seq = 0;
	gettimeofday(&start_time, NULL);
	stats.received = 0;
	stats.sent = 0;
	stats.start_time = start_time;
	stats.rtt = NULL;
	stats.total_time_ms = 0;

	parse(argc, argv, host_pos);
	printf("PING %s(%s) %i(%i) data bytes\n", host, ip_str, ICMP_PAYLOAD_SIZE, PACKET_SIZE);

	while (42) {
		seq++;

		stats.sent += send_ping(sockfd, &dest_addr,seq, &start_time);
		double ping = recv_ping(sockfd, &dest_addr, seq, &start_time, host, ip_str);
		update_stats(&stats, ping);
		sleep(1);
	}
	
	return 0;
}
