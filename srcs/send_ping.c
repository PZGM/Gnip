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

static inline void	fill_ip_header(struct iphdr *ip, struct sockaddr_in* dest)
{
	ip->version = 4;
	ip->ihl = IP_HEADER_SIZE / 4;
	ip->tos = 0;
	ip->tot_len = htons(PACKET_SIZE);
	ip->id = htons(0);
	ip->frag_off = htons(0);
	ip->ttl = TTL;
	ip->protocol = IPPROTO_ICMP;
	ip->check = 0;
	ip->saddr = INADDR_ANY;
	ip->daddr = dest->sin_addr.s_addr;
}

static inline void	fill_icmp_header(struct icmphdr* icmp, int seq)
{	
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->un.echo.id = getpid() & 0xFFFF;
	icmp->un.echo.sequence = seq;
	memset((char *)icmp + ICMP_HEADER_SIZE, 0xff, ICMP_PAYLOAD_SIZE);
	icmp->checksum = checksum((unsigned short*)icmp, PACKET_SIZE);
}