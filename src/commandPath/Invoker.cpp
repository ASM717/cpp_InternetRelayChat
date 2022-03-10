#include "Invoker.hpp"

Invoker::Invoker(Server *server) : _server(server) {
	_commands.push_back(new Pass());
}

Invoker::~Invoker() {
	std::vector<Command*>::iterator it;

	for (it = _commands.begin(); it != _commands.end(); it++) {
		delete *it;
	}
}

void Invoker::processCommand(User* sender, std::deque<std::string> args) {
	std::string commandName = args[0];
	args.pop_front();
	for (size_t i = 0; i < _commands.size(); i++) {
		if (commandName == _commands[i]->getName()) {
			_commands[i]->setServer(_server);
			_commands[i]->setSender(sender);
			_commands[i]->setArgs(args);
			try {
				_commands[i]->execute();
			} catch(const char* message) {
				sender->getReply(std::string(message));
			} catch(std::string message) {
				sender->getReply(message);
			}
			break;
		}
	}
}

bool Invoker::isCommand(std::string data) {
	std::vector<Command*>::iterator it;

	for (it = _commands.begin(); it != _commands.end(); it++) {
		if ((*it)->getName() == data) {
			return true;
		}
	}
	return false;
}

std::deque<std::string> Invoker::dataToArgs(std::string data) {
	std::stringstream	ssMsg(data);
	std::string			av;
	std::deque<std::string>	arguments;

	while (getline(ssMsg, av, ' '))
	{
		av.erase(av.find_last_not_of("\r\n") + 1);
		if (!av.empty())
			arguments.push_back(av);
	}
	return arguments;
}

void Invoker::processData(User *sender, std::string data) {
	std::deque<std::string> arguments = dataToArgs(data);

	if (!arguments.empty() && isCommand(arguments[0])) {
		std::cout << "@" << sender->getName() << " " << data; // delete
		processCommand(sender, arguments);
	}
}
