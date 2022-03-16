#pragma once

#include <iostream>
#include <map>
#include <cerrno>
#include <vector>
#include <set>
#include <queue>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/event.h>
#include <csignal>
#include <sstream>
#include <algorithm>
#include <sys/socket.h>
#include <fcntl.h>
#include <ctime>
#include <sys/types.h>
#include <netinet/in.h>
#include "fstream"
#include "Chat.hpp"
#include "Client.hpp"
#include "Hist.hpp"
#include "Information.hpp"

#define IRC_SERV "ircserv"

class Message {
private:
	std::string              m_pref;
	std::string              m_cmd;
	std::vector<std::string> m_param;

	Message();
	Message(const Message &ref);
	Message	&operator=(const Message  &ref);

	std::queue<std::string> fullFill(const std::string &string, char sep);
public:
	Message(const std::string &str);
	virtual ~Message();
	const std::string				&getPrefix() const;
	const std::string				&getCommand() const;
	const std::vector<std::string>	&getParams() const;
};

void logging(const Message &message);
bool verifyNickVal(const std::string &string);
int rplSend(const std::string &from, const Client &user, int rpl,
			const std::string& arg1= "",
			const std::string& arg2= "",
			const std::string& arg3= "");
int errSend(const Client &user, int err,
			const std::string &arg1 = "",
			const std::string &arg2 = "");
bool verifyRgx(std::string mask, std::string str);

