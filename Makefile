ifndef RESET
BLACK		:= $(shell tput -Txterm setaf 0)
RED			:= $(shell tput -Txterm setaf 1)
GREEN		:= $(shell tput -Txterm setaf 2)
YELLOW		:= $(shell tput -Txterm setaf 3)
LIGHTPURPLE	:= $(shell tput -Txterm setaf 4)
PURPLE		:= $(shell tput -Txterm setaf 5)
BLUE		:= $(shell tput -Txterm setaf 6)
WHITE		:= $(shell tput -Txterm setaf 7)
RESET		:= $(shell tput -Txterm sgr0)
endif

NAME = ft_ping

SRCS_DIR = ./srcs/

SRCS_FILES =	ft_ping.c		\
				send_ping.c		\
				recv_ping.c		\
				get_addr.c		\
				socket_fd.c		\
				checksum.c		\
				print_stats.c	\
				fill_headers.c	\
				update_stats.c	\
				free_chain.c	\
				maths.c

SRCS = $(addprefix ${SRCS_DIR}, ${SRCS_FILES})

		
OBJS = ${SRCS:.c=.o}
DEP = ${OBJS:%.o=%.d}

CC  = clang

CFLAGS = -Wall -Wextra -Werror -I includes/

SANI = -fsanitize=address -O0 -g3

all: ${NAME}
-include $(DEP)
%.o: %.c
	@printf "\e[2K\r- $@ ${YELLOW} in progress...${RESET}"
	@${CC} ${CFLAGS} -o $@ -c $<
	@printf "\e[2K\r- $@ ${GREEN} finished${RESET}\n"

$(NAME): ${OBJS}
	@${CC} -o ${NAME} ${CFLAGS} ${OBJS} 
	@echo "${GREEN}Compilation OK${RESET}"

clean:
	@echo "${RED}deleting objects${RESET}"
	@${RM} ${OBJS}

fclean:  clean
	@echo "${RED}deleting executable${RESET}"
	@${RM} ${NAME}

re: fclean all clean

san: fclean
	@${RM} ${NAME}
	@${CC} -o ${NAME} ${CFLAGS} ${SANI} ${SRCS} 
	@${RM}	${OBJS}
	@echo "${YELLOW}Execution${RESET}"

.PHONY:    all clean fclean re san exec