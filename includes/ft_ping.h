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
#include "maths.h"
 
# define ICMP_HEADER_SIZE           8
# define TTL                        64
# define IP_HEADER_SIZE			    20
# define ICMP_PAYLOAD_SIZE		    56
# define ALIGN_TIMESTAMP            4
# define ALIGN_RECEIVED_TIMESTAMP   24
# define PACKET_SIZE	            (IP_HEADER_SIZE + ICMP_HEADER_SIZE + ICMP_PAYLOAD_SIZE)

unsigned short  checksum(void *b, int len);
int             send_ping(int sockfd, struct sockaddr_in* dest_addr, int seq);
double          recv_ping(int sockfd,struct sockaddr_in* src_addr, char *host, char *ip_str);
char *          get_addr( const char *host, struct addrinfo** res, int host_pos);
int             socketfd(struct addrinfo *res, struct sockaddr_in *dest_addr);
void            print_stats(char * host, t_stats stats);
void            update_stats(t_stats *stats, double rtt);
void            free_chain(t_node * node);
void            parse(int argc, char **argv, int host_pos);
int             ft_strlen(const char *s);

void	fill_ip_header(struct iphdr *ip, struct sockaddr_in* dest);
void	fill_icmp_header(struct icmphdr* icmp, int seq);
void	fill_timestamp(void * buffer);
void    fill_buff(char * buf);

typedef struct      s_ggnip
{
    t_stats             stats;
    char                *host;
    struct addrinfo     *res;
    char                *ip_str;

    int                 sockfd;
    int                 seq;
    struct sockaddr_in  *dest_addr;

}                   t_ggnip;

#endif