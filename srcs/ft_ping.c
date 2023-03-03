#include "ft_ping.h"

static bool loop = true;

void intHandler(int dummy) {
	(void)dummy;
    loop = false;
}

int main(int argc, char **argv) {
  
  if (argc < 2) {
        exit(1);
    }

	struct addrinfo res;
	struct sockaddr_in dest_addr;
	struct timeval start_time;

	char *host = argv[1];

	char *ip_str= get_addr(host, &res);
	memset(&dest_addr, 0, sizeof(dest_addr));

	int sockfd = socketfd(&res, &dest_addr);

	int seq = 0;

   signal(SIGINT, intHandler);

	while (loop) {
		seq++;
		send_ping(sockfd, &dest_addr,seq, &start_time);
		recv_ping(sockfd, &dest_addr, seq, &start_time, host, ip_str);

		sleep(1);
	}
	print_stats(host);
	
	return 0;
}
