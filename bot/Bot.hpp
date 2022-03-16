#pragma once

#include "Request.hpp"

# define IRC_BOT "ircbot"
# define IRC_SERV "ft_irc"
# define BUFF_SIZE 1024

class Bot {
private:
	int							m_socketBot;
	std::string					m_botName;
	sockaddr_in					addr;
	socklen_t					addrLen;
	std::string					m_sender;
	std::string					m_content;
	std::string					m_serverPass;
	std::string					m_nickBot;
	char 						m_recvBuf[1024];
	int							m_port;
	static bool					work;
	std::queue<std::string>		m_requestQueue;
	std::string					m_request;
	int							m_recvBytes;

	Bot();
	Bot(const Bot &ref);
	Bot& operator=(const Bot &ref);
public:
	Bot(int port, const std::string &pass);
	~Bot();
	void botGO();
	static void sigHandler(int sign);
	void requestQueue(const std::string& req);
};

