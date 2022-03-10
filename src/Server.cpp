#include "Server.hpp"

Server::Server(int port, std::string pass) : port(port), password(pass) {
//	this->ip = "127.0.0.1";
	this->ip = "127.0.0.1";
	std::cout << "ip = " << ip << std::endl;
	std::cout << "port = " << port << std::endl;
	std::cout << "pass = " << pass << std::endl;
}

//Server::Server(const Server &ref) {
//
//}

//Server &Server::operator=(const Server &ref) {
//
//}

Server::~Server() {

}

//void Server::cmdMessage(const Message &msg, Client &client) {
//	switch (msg) {
//		case "PASS":
//			if (msg.get_params().size() == 0)
//				send_error(client, 461, msg.get_command());
//			else if (client.get_flags() & 0b00000001)
//				send_error(client, 462);
//			else
//				client.set_password(msg.get_params()[0]);
//
//		case "USER":
//			if (msg.get_params().size() < 4)
//				send_error(client, 461, msg.get_command());
//			else if (client.get_flags() & 0b00000001)
//				send_error(client, 462);
//			else
//			{
//				client.set_username(msg.get_params()[0]);
//				client.set_realname(msg.get_params()[3]);
//			}
//			return connect_member(client);
//
//		case "NICK":
//			if (msg.get_params().size() == 0)
//				send_error(client, 461, msg.get_command());
//			else if (!is_valid_nick(msg.get_params()[0]) || msg.get_params()[0] == "ircserv")
//				send_error(client, 432, msg.get_params()[0]);
//			else if (is_exist_member(msg.get_params()[0]))
//				send_error(client, 433, msg.get_params()[0]);
//			else
//			{
//				if (client.get_flags() & 0b00000001)
//				{
//					notify(client, ":" + client.get_prefix() + " " + msg.get_command() + " " + msg.get_params()[0] + "\n");
//					history.add_user(client);
//				}
//				client.set_nick(msg.get_params()[0]);
//			}
//			return connect_member(client);
//	}
//}

void Server::go(Socket &sock) {

	//оформить сигналы

	sock.getAddress().sin_family = AF_INET;
	sock.getAddress().sin_port = htons(port);
	sock.getAddress().sin_addr.s_addr = htonl(INADDR_ANY);
	memset(sock.getAddress().sin_zero, 0, sizeof(sock.getAddress().sin_zero));
	sock.sockLen = sizeof(sock.getAddress());



	//fjvnsdklcdfn


	
	int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {
		std::cerr << "Error: Socket failed!" << std::endl;
		exit(1);
	}

	int	flag = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1
		|| bind(sock_fd, reinterpret_cast<struct sockaddr *>(&sock.getAddress()), sock.getLen()) == -1) {
		std::cerr << "Error: setsockopt failed!" << std::endl;
		exit(1);
	}

	//20???
	if (listen(sock_fd, 20) == -1) {
		std::cerr << "Error: listen failed!" << std::endl;
		exit(1);
	}
	fcntl(sock_fd, F_SETFL, O_NONBLOCK);

	while (true) {
		int client_fd;
		//могут быть траблы
		if ((client_fd = accept(sock_fd, reinterpret_cast<struct sockaddr*>(&sock.getAddress()),
								reinterpret_cast<socklen_t *>(sock.getLen()))) >= 0)
		{
			ip = inet_ntoa(sock.getAddress().sin_addr);
			struct pollfd poll_fd;
			poll_fd.fd = client_fd;
			poll_fd.events = POLLIN;
			poll_fd.revents = 0;
			client_fds.push_back(poll_fd);
			members.push_back(new Client(client_fd,SERVER_NAME));
		}
	}
}