#include "Bot.hpp"

int main(int argc, char** argv) {

	int port;

	if (argc != 3) {
		std::cerr << "Error: You should use ./bot <m_port> <m_passsword of ircserv>" << std::endl;
		exit(1);
	}
	port = atoi(argv[1]);
	if (port < 1024 || port > 65535) {
		std::cerr << "Error: m_port parse failed!" << std::endl;
		exit(1);
	}
	try {
		Bot bot(port, argv[2]);
		bot.botGO();
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}