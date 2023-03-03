#include "ft_ping.h"

void send_ping(int sockfd, struct sockaddr_in* dest_addr, int seq, struct timeval *start_time) {
	char send_buf[MAX_PACKET_SIZE];
	
	struct icmphdr *icmp = (struct icmphdr*)send_buf;
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->un.echo.id = getpid() & 0xFFFF;
	icmp->un.echo.sequence = seq;
	memset(send_buf + ICMP_HEADER_SIZE, 0xff, MAX_PACKET_SIZE - ICMP_HEADER_SIZE);
	icmp->checksum = checksum((unsigned short*)icmp, MAX_PACKET_SIZE);
	
	if (gettimeofday(start_time, NULL) < 0){
		write(2, "Error: Failed to get time of day\n", 34);
		exit(1);
	}

	if (sendto(sockfd, icmp, ICMP_HEADER_SIZE + MAX_PACKET_SIZE - ICMP_HEADER_SIZE, 0, (struct sockaddr*)dest_addr, sizeof(*dest_addr)) < 0) {
		perror("Failed to send ICMP echo request");
		exit (1);
	}
}