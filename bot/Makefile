NAME	= bot

SRCS	= *.cpp
INC		= *.hpp
CC		= c++
CFLAGS	= -g -Wall -Wextra -Werror -std=c++98
RM		= rm -rf

OBJS	= $(SRCS:.cpp=.o)

$(NAME):$(OBJS) $(INC)
			$(CC) $(CFLAGS) $(SRCS) -o $(NAME)
all: $(NAME)
clean:
			$(RM) $(OBJS)
fclean: clean
			$(RM) $(NAME)
re: fclean all
.PHONY: all clean fclean re
