#include "Command.hpp"

Command::Command() :
	_name(""), _description(""), _server(NULL), _sender(NULL) {

}

Command::~Command() {

}

std::string Command::getName() const {
	return _name;
}

std::string Command::getDescription() const {
	return _description;
}

void Command::setSender(User* sender) {
	_sender = sender;
}

void Command::setServer(Server* server) {
	_server = server;
}

void Command::setArgs(std::deque<std::string> args) {
	_args = args;
}
