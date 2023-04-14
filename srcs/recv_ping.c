#include "ft_ping.h"

int validate_response(struct icmphdr* icmp) {
	const int pid = getpid() & 0xFFFF;
	if (icmp->type != 0)
		return false;
	if (icmp->code != 0)
		return false;
	if (icmp->un.echo.id != pid)
		return false;
	return true;
}

double recv_ping(int sockfd,struct sockaddr_in* src_addr, char * host, char * ip_str) {
	char recv_buf[PACKET_SIZE];

	struct iovec iov = {
		.iov_base = recv_buf,
		.iov_len = sizeof(recv_buf)
	};
	struct msghdr msg = {
		.msg_name = src_addr,
		.msg_namelen = sizeof(src_addr),
		.msg_iov = &iov,
		.msg_iovlen = 1
	};
	if (recvmsg(sockfd, &msg, 0) < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			printf("Request timed out");
		} else {
			perror("Failed to receive ICMP echo reply");
			exit(1);
		}
		return -1;
	} else {
		void * st = recv_buf + IP_HEADER_SIZE + ICMP_HEADER_SIZE + ALIGN_RECEIVED_TIMESTAMP;
		struct timeval * start_time = (struct timeval *)st;
		struct timeval end_time;
		struct iphdr	*ip = (struct iphdr *)recv_buf;
		struct icmphdr *icmp = (struct icmphdr *)((uint32_t *)ip + ip->ihl);

	if (ip->protocol == 1)
		icmp = (struct icmphdr *)((uint32_t *)ip + ip->ihl);
	else {
		perror("Failed to receive ICMP echo reply");
		exit(1);
	}
			
		if (!validate_response(icmp)) {
			return -1;
		}

		if (gettimeofday(&end_time, NULL) < 0) {
			write(2, "Error: Failed to get time of day\n", 34);
			exit(1);
		}
		double ping = (end_time.tv_sec - start_time->tv_sec) * 1000.0 + (end_time.tv_usec - start_time->tv_usec) / 1000.0;
		printf("%zu bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3f ms\n", iov.iov_len - IP_HEADER_SIZE, host, ip_str, icmp->un.echo.sequence, ip->ttl, ping);
		return ping;
	}
}