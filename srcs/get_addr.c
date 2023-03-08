#include "ft_ping.h"

char *get_addr( const char *host, struct addrinfo** res_ptr) {
	struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4 only
    hints.ai_socktype = SOCK_RAW; // Raw socket for ICMP packets
    hints.ai_protocol = IPPROTO_ICMP; // ICMP protocol

	int ret = getaddrinfo(host, NULL, &hints, res_ptr);
	if (ret < 0){
		write(2, "Error: Failed to get hostname\n", 30);
		exit(1); 
	}

    if (ret != 0) {
        write(2, "Failed to get address info\n", 28);
        exit(1);
    }

    struct addrinfo *res = *res_ptr;

    char ip[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(((struct sockaddr_in *)(res->ai_addr))->sin_addr), ip, INET_ADDRSTRLEN) == NULL) {
        write(2, "Failed to convert IP address to string\n", 40);
        exit(1);
    }
    char *ip_str = malloc(sizeof(char) * (INET_ADDRSTRLEN + 1));
    strcpy(ip_str, ip);

    return ip_str;
}