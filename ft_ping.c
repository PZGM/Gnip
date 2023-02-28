#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/ip_icmp.h>


int main(int argc, char **argv) {
  
  if (argc < 2) {
        exit(1);
    }

	
	struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4 only
    hints.ai_socktype = SOCK_RAW; // Raw socket for ICMP packets
    hints.ai_protocol = IPPROTO_ICMP; // ICMP protocol

    const char *host = argv[1];	
	struct addrinfo *res;
	int ret = getaddrinfo(host, NULL, &hints, &res);
	if (ret < 0){
		write(2, "Error: Failed to ge hostname\n", 30);
		exit(1); 
	}

    if (ret != 0) {
        write(2, "Failed to get address info\n", 28);
        exit(1);
    }

    char ip_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(((struct sockaddr_in *)(res->ai_addr))->sin_addr), ip_str, INET_ADDRSTRLEN) == NULL) {
        write(2, "Failed to convert IP address to string\n", 40);
        exit(1);
    }

    printf("Pinging %s (%s):\n", host, ip_str);

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
	write(1, "yeah!\n", 6);
	
	struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    memcpy(&dest_addr.sin_addr, &(((struct sockaddr_in *)(res->ai_addr))->sin_addr), sizeof(dest_addr.sin_addr));
	
	if (sendto(sockfd, "coucou", 6, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
		write(2, "Error: Failed to send payload\n", 31);
		exit(1);
	}

	free(res);
}
