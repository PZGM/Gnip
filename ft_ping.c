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
#include <errno.h>

#define MAX_PACKET_SIZE 64
#define ICMP_HEADER_SIZE 8


unsigned short checksum(void *b, int len) {    
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;


    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;

    if (len == 1)
        sum += *(unsigned char*)buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

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

void recv_ping(int sockfd,struct sockaddr_in* src_addr, int seq, struct timeval *start_time) {
	char recv_buf[MAX_PACKET_SIZE];
	struct iovec iov = {
		.iov_base = recv_buf,
		.iov_len = sizeof(recv_buf)
	};
	struct msghdr msg = {
		.msg_name = &src_addr,
		.msg_namelen = sizeof(&src_addr),
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
			printf("recv : %s", recv_buf);

		struct timeval end_time;
		if (gettimeofday(&end_time, NULL) < 0) {
			write(2, "Error: Failed to get time of day\n", 34);
			exit(1);
		}
		double ping = (end_time.tv_sec - start_time->tv_sec) * 1000.0 + (end_time.tv_usec - start_time->tv_usec) / 1000.0;
		printf("da ping is %.2f ms\n", ping);
	}
}

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
	struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        fprintf(stderr, "Failed to set receive timeout\n");
        exit(EXIT_FAILURE);
    }

	struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    memcpy(&dest_addr.sin_addr, &(((struct sockaddr_in *)(res->ai_addr))->sin_addr), sizeof(dest_addr.sin_addr));
	
	int seq = 0;

	struct timeval start_time;

	while (1) {
		send_ping(sockfd, &dest_addr,seq, &start_time);
		recv_ping(sockfd, &dest_addr, seq, &start_time);
		seq++;
		sleep(1);
	}

	free(res);
}
