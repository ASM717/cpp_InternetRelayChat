#pragma once
#include "../Server.hpp"
#include "Command.hpp"

class Invoker {
private:
	Server                *_server;
	std::vector<Command*> _commands;

public:
	Invoker(Server* server);
	virtual ~Invoker();

	void			processData(User* sender, std::string msg);
	void			processCommand(User* sender, std::deque<std::string> arguments);
	bool			isCommand(std::string data);
	std::deque<std::string>	dataToArgs(std::string data);
};