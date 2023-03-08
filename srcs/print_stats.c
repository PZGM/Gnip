#include "ft_ping.h"

void print_stats(char * host, t_stats stats) {
    struct timeval now;
    gettimeofday(&now, NULL);

    int lost_percentage = (stats.sent - stats.received) / stats.sent * 100;
    
    int duration = (now.tv_sec - stats.start_time.tv_sec) * 1000 + (now.tv_usec - stats.start_time.tv_usec) / 1000;

    t_node * cur = stats.rtt;
    double min = 0;
    double max = 0;
    double mdev = 0;
    double mean = stats.total_time_ms / stats.received;

    while (cur) {
        if (cur->data > max) {
            max = cur->data;
        }
        if (!min || cur->data < min) {
            min = cur->data;
        }
        mdev += ft_pow(cur->data - mean, 2);
        cur = cur->next;
    }
    mdev = ft_sqrt(mdev / stats.received);

    printf("\n--- %s ping statistics ---\n", host);
    printf("%d packets transmitted, %d received, %d %% packet loss, time %d ms\n", stats.sent, stats.received, lost_percentage, duration);
    printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", min, max, mean, mdev);
    free_chain(stats.rtt);
}