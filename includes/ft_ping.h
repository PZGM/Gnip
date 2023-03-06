#ifndef FT_PING_H
# define FT_PING_H

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
#include <signal.h>
#include <stdbool.h>
#include "chain.h"
#include "stats.h"

# define ICMP_HEADER_SIZE       8
# define TTL                    64
# define IP_HEADER_SIZE			20
# define ICMP_PAYLOAD_SIZE		36
# define PACKET_SIZE	        (IP_HEADER_SIZE + ICMP_HEADER_SIZE + ICMP_PAYLOAD_SIZE)

unsigned short checksum(void *b, int len);
int send_ping(int sockfd, struct sockaddr_in* dest_addr, int seq, struct timeval *start_time);
int recv_ping(int sockfd,struct sockaddr_in* src_addr, int seq, struct timeval *start_time, char *host, char *ip_str);
char *get_addr( const char *host, struct addrinfo** res);
int socketfd(struct addrinfo *res, struct sockaddr_in *dest_addr);
void print_stats(char * host, t_stats stats);
void update_stats(t_stats *stats, double rtt);
void free_chain(t_node * node);

static inline void	fill_ip_header(struct iphdr *ip, struct sockaddr_in* dest);
static inline void	fill_icmp_header(struct icmphdr* icmp, int seq);

#endif