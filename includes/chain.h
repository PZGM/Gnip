#ifndef CHAIN_H
# define CHAIN_H

typedef struct s_node {
                void * next;
                double data;
                struct timeval * start_time;
                int seq;
}               t_node;

#endif