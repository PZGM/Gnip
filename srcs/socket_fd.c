#include "ft_ping.h"

int socketfd(struct addrinfo *res, struct sockaddr_in *dest_addr) {

	int sockfd;
	int ttl = 64;
	
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0)	{
		write(2, "Error: Socket creation failed\n", 30);
		exit(1); 
	}
	if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {	
		write(2, "Error: Failed to set ttl\n", 26);
		exit(1);
	}

    dest_addr->sin_family = AF_INET;
    memcpy(&dest_addr->sin_addr, &(((struct sockaddr_in *)(res->ai_addr))->sin_addr), sizeof(dest_addr->sin_addr));

	return sockfd;
}