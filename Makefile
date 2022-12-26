CC = cc

CFLAGS = -Wall -Werror -Wextra -fsanitize=thread

SRCS = main.c init.c actions.c tools.c more_tools.c errors.c

OBJS = $(SRCS:.c=.o)

NAME = philo

all : $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ 
clean : 
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all
