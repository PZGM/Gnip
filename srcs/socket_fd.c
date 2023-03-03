#include "ft_ping.h"

int socketfd(struct addrinfo *res, struct sockaddr_in *dest_addr) {

	int sockfd;
	int ttl = 64;
	
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0)	{
		write(2, "Error: Socket creation failed. Try to sudo\n", 44);
		exit(1); 
	}
	if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {	
		write(2, "Error: Failed to set ttl\n", 26);
		exit(1);
	}
	/*struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        fprintf(stderr, "Failed to set receive timeout\n");
        exit(EXIT_FAILURE);
    }*/

    dest_addr->sin_family = AF_INET;
    memcpy(&dest_addr->sin_addr, &(((struct sockaddr_in *)(res->ai_addr))->sin_addr), sizeof(dest_addr->sin_addr));

	return sockfd;
}