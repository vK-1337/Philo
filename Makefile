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

$(LIBFT): libft
	$(MAKE) -C ./libft

.c.o:
	$(CC) $(CFLAGS) -c $< -o ${<:.c=.o}

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C ./libft fclean

re: fclean
	make all

libft:
	git clone https://github.com/vK-1337/42-libft.git $@

.PHONY: all clean fclean re
