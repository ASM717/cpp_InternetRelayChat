#pragma once

#include "Server.hpp"

class Challenge {
private:
	Server* _server;
	vector<CommandMessage*> _commands;

public:
	Challenge(Server* server);
	virtual ~Challenge();
};