#include "includes/Server.hpp"

int main(int argc, char** argv)
{
	if (argc != 3) {
		std::cerr << "You should use: ./ircserv <port> <pass>" << std::endl;
		return (-1);
	}
	int port = atoi(argv[1]);
	if (port < 1024 || port > 65535) {
		std::cerr << "Error: port is invalid" << std::endl;
		return (-1);
	}
	try {
		Server server(port, std::string(argv[2]));
		server.go();
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}

