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

#define MAX_PACKET_SIZE 64
#define ICMP_HEADER_SIZE 8

unsigned short checksum(void *b, int len);
void send_ping(int sockfd, struct sockaddr_in* dest_addr, int seq, struct timeval *start_time);
void recv_ping(int sockfd,struct sockaddr_in* src_addr, int seq, struct timeval *start_time, char *host, char *ip_str);
char *get_addr( const char *host, struct addrinfo** res);
int socketfd(struct addrinfo *res, struct sockaddr_in *dest_addr);
void print_stats(char * host);

#endif