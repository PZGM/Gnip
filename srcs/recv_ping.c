#include "ft_ping.h"

void recv_ping(int sockfd,struct sockaddr_in* src_addr, int seq, struct timeval *start_time, char * host, char * ip_str) {
	char recv_buf[MAX_PACKET_SIZE];

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
			printf("Request timed out for ICMP sequence %d\n", seq);
		} else {
			perror("Failed to receive ICMP echo reply");
			exit(1);
		}
	} else {
		struct timeval end_time;
		if (gettimeofday(&end_time, NULL) < 0) {
			write(2, "Error: Failed to get time of day\n", 34);
			exit(1);
		}
		double ping = (end_time.tv_sec - start_time->tv_sec) * 1000.0 + (end_time.tv_usec - start_time->tv_usec) / 1000.0;
		printf("%zu bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3f ms\n", iov.iov_len, host, ip_str, seq, 69, ping);
	}
}