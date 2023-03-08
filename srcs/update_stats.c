#include "ft_ping.h"

void update_stats(t_stats *stats, double rtt) {
    if (rtt > 0) {
        stats->received++;
        stats->total_time_ms += rtt;
        t_node *node = malloc(sizeof(t_node));
        node->data = rtt;
        node->next = NULL;

        t_node *cur = stats->rtt;
        if (!cur) {
            stats->rtt = node;
        }
        else
        {
            while(cur->next)
            {
                cur = cur->next;
            }
            cur->next = node;
        } 
    }
}