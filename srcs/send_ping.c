#include "ft_ping.h"

int send_ping(int sockfd, struct sockaddr_in* dest_addr, int seq) {
	
	char send_buf[PACKET_SIZE];

	fill_ip_header((struct iphdr*)send_buf, dest_addr);
	fill_buff(send_buf + ICMP_HEADER_SIZE);
	fill_timestamp(send_buf + IP_HEADER_SIZE + ICMP_HEADER_SIZE + ALIGN_TIMESTAMP);
	fill_icmp_header((struct icmphdr *)send_buf, seq);

	if (sendto(sockfd, send_buf, PACKET_SIZE, 0, (struct sockaddr*)dest_addr, sizeof(*dest_addr)) < 0) {
		perror("Failed to send ICMP echo request");
		exit (1);
	}

	return 1;
}