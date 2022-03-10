#pragma once

#include "../Server.hpp"
#include "../Client.hpp"

class ACommand {
protected:
	Server *_server;

public:
	ACommand(Server *server);
	virtual ~ACommand();

	virtual void execute(Client *client, std::vector<std::string> arguments) = 0;
};