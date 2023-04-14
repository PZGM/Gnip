#include "ft_ping.h"

void	fill_ip_header(struct iphdr *ip, struct sockaddr_in* dest)
{
	ip->version = 4;
	ip->ihl = IP_HEADER_SIZE / 4;
	ip->tos = 0;
	ip->tot_len = PACKET_SIZE;
	ip->id = 0;
	ip->frag_off = 0;
	ip->ttl = TTL;
	ip->protocol = IPPROTO_ICMP;
	ip->check = 0;
	ip->saddr = INADDR_ANY;
	ip->daddr = dest->sin_addr.s_addr;
}

void	fill_icmp_header(struct icmphdr* icmp, int seq)
{	
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->un.echo.id = getpid() & 0xFFFF;
	icmp->un.echo.sequence = seq;
	icmp->checksum = checksum((unsigned short*)icmp, PACKET_SIZE);
}

void	fill_timestamp(void * buf) {
	gettimeofday(buf, NULL);
}

void	fill_buff(char * buf) {
	memset(buf, 0xff, ICMP_PAYLOAD_SIZE);
}