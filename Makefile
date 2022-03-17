NAME	= ircserv

SRCS	= main.cpp srcs/Chat.cpp srcs/Client.cpp srcs/Hist.cpp \
			srcs/Message.cpp srcs/Server.cpp srcs/Information.cpp

INC		= includes/Chat.hpp includes/Client.hpp includes/Hist.hpp \
			includes/Message.hpp includes/Server.hpp includes/Information.hpp
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

