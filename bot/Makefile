NAME	= bot

SRCS	= main.cpp Bot.cpp Request.cpp
INC		= Bot.hpp Request.hpp
CC		= c++
CFLAGS	= -g -Wall -Wextra -Werror -std=c++98
RM		= rm -rf

OBJS = $(SRCS:.cpp=.o)
all: $(NAME)
$(NAME):$(OBJS) $(INC)
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
$(OBJS): %.o: %.cpp
			$(CC) $(CFLAGS) -o $@ -c $<

clean:
			$(RM) $(OBJS)
fclean: clean
			$(RM) $(NAME)
re: fclean all
.PHONY: all clean fclean re
