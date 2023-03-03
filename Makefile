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

SRCS =	./srcs/ft_ping.c		\
		./srcs/send_ping.c		\
		./srcs/recv_ping.c		\
		./srcs/get_addr.c		\
		./srcs/socket_fd.c			\
		./srcs/checksum.c		\

		
OBJS = ${SRCS:.c=.o}

CC  = clang

CFLAGS = -Wall -Wextra -Werror -I includes/

SANI = -fsanitize=address -O0 -g3 --std=c98

$(NAME): ${OBJS} Makefile
	@echo "\r\r${YELLOW}Compilation...${RESET}"
	@${CC} -o ${NAME} ${CFLAGS} ${SRCS} 
	@echo "${GREEN}Compilation OK${RESET}"
	@${RM}	${OBJS}

all:  ${NAME}

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