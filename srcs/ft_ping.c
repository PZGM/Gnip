#include "ft_ping.h"

static volatile bool loop = true;

void intHandler(int dummy) {
	(void)dummy;
    loop = false;
}

int main(int argc, char **argv) {
  
  	if (argc < 2)
	{
        exit(1);
    }
	
	int x = 0;
	int host_pos = -1;
	char *host;
	while (++x < argc)
	{
		if (argv[x][0] != '-' || !argv[x][1])
		{
			host = argv[x];
			host_pos = x;
		}
	}
	signal(SIGINT, intHandler);
	struct addrinfo *res;
	struct sockaddr_in dest_addr;
	struct timeval start_time;

	char * ip_str = get_addr(host, &res);
	memset(&dest_addr, 0, sizeof(dest_addr));

	int sockfd = socketfd(res, &dest_addr);

	int seq = 0;
	gettimeofday(&start_time, NULL);
	t_stats stats = {
		.received = 0,
		.sent = 0,
		.start_time = start_time,
		.rtt = NULL,
		.total_time_ms = 0
	};

	parse(argc, argv, host_pos);
	printf("PING %s(%s) 56(84) data bytes\n", host, ip_str);

	while (loop) {
		seq++;

		stats.sent += send_ping(sockfd, &dest_addr,seq, &start_time);
		double ping = recv_ping(sockfd, &dest_addr, seq, &start_time, host, ip_str);
		update_stats(&stats, ping);
		sleep(1);
	}

	print_stats(host, stats);

	free(res);
	free(ip_str);
	
	return 0;
}
