#ifndef STATS_H
# define STATS_H

#include "ft_ping.h"

typedef struct		s_stats
{
	int							sent;
	int							received;
	struct timeval	start_time;
	t_node 					*rtt;
	double					total_time_ms;
} 					t_stats;

#endif