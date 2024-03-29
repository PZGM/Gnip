NAME = ft_ping

SRCS_DIR = ./srcs/

CC = clang

CFLAGS = -Wall -Wextra -Werror ${INCLUDES}

INCLUDES =  -I./includes/

SANI = -fsanitize=address -O0 -g3

SRCS_FILES =    ft_ping.c        \
                send_ping.c        \
                recv_ping.c        \
                get_addr.c        \
                socket_fd.c        \
                checksum.c        \
                print_stats.c    \
                fill_headers.c    \
                update_stats.c    \
                free_chain.c    \
                maths.c            \
                parsing.c           \
                utils.c

SRCS = $(addprefix ${SRCS_DIR}, ${SRCS_FILES})

        
OBJS = ${SRCS:.c=.o}

all: ${NAME}

$(NAME): ${OBJS}
	${CC} ${OBJS} ${CFLAGS} -o ${NAME}

clean:
	${RM} ${OBJS} ${DEP}

fclean:  clean
	${RM} ${NAME}

re: fclean all clean

san: fclean
	${RM} ${NAME}
	${CC} -o ${NAME} ${CFLAGS} ${SANI} ${SRCS} 

.PHONY: all clean fclean re san exec
