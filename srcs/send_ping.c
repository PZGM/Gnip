#include "ft_ping.h"

int send_ping(int sockfd, struct sockaddr_in* dest_addr, int seq, struct timeval *start_time) {
	
	char send_buf[PACKET_SIZE];

	fill_ip_header((struct iphdr*)send_buf, dest_addr);
	fill_icmp_header((struct icmphdr*)send_buf, seq);


	if (gettimeofday(start_time, NULL) < 0){
		write(2, "Error: Failed to get time of day\n", 34);
		exit(1);
	}

	if (sendto(sockfd, send_buf, PACKET_SIZE, 0, (struct sockaddr*)dest_addr, sizeof(*dest_addr)) < 0) {
		perror("Failed to send ICMP echo request");
		exit (1);
	}

	return 1;
}