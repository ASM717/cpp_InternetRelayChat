#ifndef FT_IRC_UTILS_HPP
#define FT_IRC_UTILS_HPP

# include <iostream>
# include <map>
# include <cerrno>
# include <vector>
# include <set>
# include <queue>
# include <unistd.h>
# include <cstdlib>
# include <algorithm>
# include <sys/socket.h>
# include <fcntl.h>
# include <ctime>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/poll.h>
# include <sys/event.h>
# include <csignal>
# include <sstream>
# include "fstream"
# include "Chat.hpp"
# include "Client.hpp"
# include "Message.hpp"
# include "History.hpp"
# include "UserInfo.hpp"
# include "Hash.hpp"

# define RED		"\033[31m"
# define RESET	"\033[0m"

# define SERVER_NAME "ircserv"

class Message;

int		send_reply(const std::string &from, const Client &user, int rpl, const std::string& arg1="", const std::string& arg2="", \
				 const std::string& arg3="", const std::string& arg4="", const std::string& arg5="", const std::string& arg6="", \
				 const std::string& arg7="", const std::string& arg8="");
int		send_error(const Client &user, int err, const std::string &arg1 = "", const std::string &arg2 = "");
bool	is_equal_to_regex(std::string mask, std::string str);
void	log_message(const Message &msg);
bool	is_valid_nick(const std::string &nick);

#endif
