#pragma once

#include <iostream>
#include <map>
#include <cerrno>
#include <vector>
#include <set>
#include <queue>
#include <unistd.h>
#include <cstdlib>
#include <algorithm>
#include <sys/socket.h>
#include <fcntl.h>
#include <ctime>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/event.h>
#include <csignal>
#include <sstream>
#include <fstream>

class Request {
private:
	std::string					m_request;
	std::queue<std::string>		m_paramsQueue;
	std::vector<std::string>	m_params;
	std::string					m_prefix;
	std::string					m_command;

	Request();
	Request(const Request &ref);
	Request& operator=(const Request &ref);

public:
	Request(const std::string& Request);
	~Request();

	const std::string& getPrefix() const;
	const std::string& getCommand() const;
	const std::vector<std::string>& getParams() const;
};