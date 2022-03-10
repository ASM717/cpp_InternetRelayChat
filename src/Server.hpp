#pragma once

#include "Socket.hpp"

class Socket;

class Server {
private:
	Server();

	int              port;
	std::string      password;
//	char             *ip;
	std::string      ip;
	std::string      _serverName;
//	std::vector<struct pollfd>	client_fds;

	std::vector<pollfd>		_pollfds;
	std::vector<User *>		_users;
	std::vector<Channel *>	_channels;

	Invoker					*_Invoker;
//	MessageHandler *_commandHandler;

public:
	Server(int port, std::string pass);
//	Server(const Server &ref);
//	Server &operator=(const Server &ref);
	~Server();
	void go(Socket &sock);

};

