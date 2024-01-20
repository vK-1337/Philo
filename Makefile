SRCS = philo.c \
				parsing.c \

OBJS = $(SRCS:.c=.o)

NAME = philo

CFLAGS = -Wall -Wextra -g3 -pthread

# colors
GREEN = \033[0;32m
NC = \033[0m
YELLOW = \e[0;93m

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(NAME)
	@echo "	 			+---------------------+\n \
					|  $(YELLOW)    PHILO $(GREEN)[OK]$(NC)     |\n \
					+---------------------+"

.c.o:
	$(CC) $(CFLAGS) -c $< -o ${<:.c=.o}

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean
	make all

.PHONY: all clean fclean re
