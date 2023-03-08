#include "ft_ping.h"

void free_chain(t_node * node) {
    if (node && node->next) {
        free_chain(node->next);
        node->next = NULL;
    }
    if (node) {
        free(node);
    }
}