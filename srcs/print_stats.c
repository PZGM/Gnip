#include "ft_ping.h"

void print_stats(char * host, t_stats stats) {
    struct timeval now;
    gettimeofday(&now, NULL);

    int lost_percentage = (stats.sent - stats.received) / stats.sent * 100;
    
    int duration = (now.tv_sec - stats.start_time.tv_sec) * 1000 + (now.tv_usec - stats.start_time.tv_usec) / 1000;

    printf("--- %s ping statistics ---\n", host);
    printf("%d packets transmitted, %d received, %d %% packet loss, time %d ms\n", stats.sent, stats.received, lost_percentage, duration);

    free_chain(stats.rtt);
}