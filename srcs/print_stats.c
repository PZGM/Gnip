#include "ft_ping.h"

void print_stats(char * host, t_stats stats) {
    struct timeval now;
    gettimeofday(&now, NULL);

    int lost_percentage = (stats.sent - stats.received) / stats.sent * 100;
    
    int duration = (now.tv_sec - stats.start_time.tv_sec) * 1000 + (now.tv_usec - stats.start_time.tv_usec) / 1000;

    t_node * cur = stats.rtt;
    double min = 0;
    double max = 0;
    double mean = 0;
    while (cur) {
        if (cur->data > max) {
            max = cur->data;
        }
        if (!min || cur->data < min) {
            min = cur->data;
        }
        mean += cur->data;
        printf("dta -> %f\n", cur->data);
        cur = cur->next;
    }

    mean /= stats.received;

    printf("--- %s ping statistics ---\n", host);
    printf("%d packets transmitted, %d received, %d %% packet loss, time %d ms\n", stats.sent, stats.received, lost_percentage, duration);
    printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms", min, max, mean, 0.0);
    free_chain(stats.rtt);
}