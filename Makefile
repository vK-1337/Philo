SRCS = src/parsing.c \
			src/main.c \
			src/parsing_utils.c \
			src/philo.c \
			src/philo_utils.c \
			src/actions.c \
			src/init.c \
			src/getters.c \
			src/setters.c \
			src/threads.c \
			src/error.c \

BONUS_SRCS = src_bonus/main.c \
						src_bonus/actions.c \
						src_bonus/init.c \
						src_bonus/children.c \
						src_bonus/getters.c \
						src_bonus/setters.c \
						src_bonus/parsing.c \
						src_bonus/parsing_utils.c \
						src_bonus/philo.c \
						src_bonus/philo_utils.c \
						src_bonus/errors.c \

OBJS = $(SRCS:.c=.o)

BONUS_OBJS = $(BONUS_SRCS:.c=.o)

NAME = philo

BONUS = philo_bonus

CFLAGS = -Wall -Wextra -Werror -pthread -g3

# colors
GREEN = \033[0;32m
NC = \033[0m
YELLOW = \e[0;93m

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(NAME)
	@echo "	 			+---------------------+\n \
					|  $(YELLOW)    PHILO $(GREEN)[OK]$(NC)     |\n \
					+---------------------+"

$(BONUS): $(BONUS_OBJS)
	$(CC) $(LDFLAGS) $(BONUS_OBJS) -o $(BONUS)
	@echo "	 			+---------------------+\n \
					|  $(YELLOW) PHILO_BONUS $(GREEN)[OK]$(NC)  |\n \
					+---------------------+"

.c.o:
	$(CC) $(CFLAGS) -c $< -o ${<:.c=.o}

all: $(NAME) $(BONUS)

bonus: $(BONUS)

clean:
	rm -f $(OBJS)
	rm -f $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME)
	rm -f $(BONUS)

re: fclean
	make all

.PHONY: all clean fclean re
