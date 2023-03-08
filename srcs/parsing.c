#include "ft_ping.h"

void print_help() {
    printf("\nUsage\n ping [options] <destination>\n\nOptions:\n <destination>      dns name or ip address\n-v                 verbose output\n\nFor more details see ping(8)\n");
}

void parse(int argc, char **argv, int host_pos) {
    int x = 0;
    while (++x < argc) {
        char *opt = argv[x];
        int y = 1;
        if (x == host_pos)
        {
            continue;
        }
        if (*opt != '-' || !opt[1]) {
            print_help();
            exit(1);
        }
        while (opt[y]) {
            if (opt[y] != 'v')
            {
                if (opt[y] != 'h') {
                    printf("invalide option -- %c\n\n",opt[y]);
                }
                print_help();
                exit(0);
            }
            y++;
        }
    }
}