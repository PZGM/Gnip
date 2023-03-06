#include "ft_ping.h"

int main(int argc, char **argv) {
  
  if (argc < 2) {
        exit(1);
    }

	struct addrinfo *res;
	struct sockaddr_in dest_addr;
	struct timeval start_time;

	char * ip_str = get_addr(argv[1], &res);
	memset(&dest_addr, 0, sizeof(dest_addr));

	int sockfd = socketfd(res, &dest_addr);

	int seq = 0;

	while (++seq) {

		send_ping(sockfd, &dest_addr,seq, &start_time);
		recv_ping(sockfd, &dest_addr, seq, &start_time, argv[1], ip_str);

		sleep(1);
	}

	free(&res);
	
	return 0;
}
