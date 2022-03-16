#include "Bot.hpp"

bool Bot::work = true;

Bot::Bot(int port, const std::string& pass) :
		m_botName(IRC_BOT), m_serverPass(pass), m_nickBot(IRC_BOT), m_port(port) {
	std::cout << "BOT is ready to use!" << std::endl;
}

Bot::~Bot()
{
	close(m_socketBot);
}

void Bot::sigHandler(int sign)
{
	(void)sign;
	work = false;
}

std::string getSender(const std::string &prefix)
{
	char sender[512] = {0};
	sscanf(prefix.c_str(), "%[^!]", sender);

	return (std::string(sender));
}

void Bot::requestQueue(const std::string& req)
{
	std::string::const_iterator slash_n_it;

	for (std::string::const_iterator it = req.begin(); it != req.end(); ) {
		for (; it != req.end() && *it == '\n'; ++it);
		slash_n_it = std::find(it, req.end(), '\n');
		if (it != req.end())
		{
			m_requestQueue.push(std::string(it, slash_n_it + 1));
			std::cout << std::string(it, slash_n_it + 1) << std::endl;
			it = slash_n_it;
		}
	}
}

void Bot::botGO() {

	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
	addrLen = sizeof(addr);

	if ((m_socketBot = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "Error: socket initialization error!" << std::endl;
		exit(1);
	}
	if (connect(m_socketBot, reinterpret_cast<struct sockaddr*>(&addr), addrLen) < 0) {
		std::cerr << "Error: connection error!" << std::endl;
		exit(1);
	}
	fcntl(m_socketBot, F_SETFL, O_NONBLOCK);

	std::string log[3];

	log[0] = "PASS " + m_serverPass + "\n";
	log[1] = "USER " + m_botName + " 127.0.0.1" + " ft_irc" + " " + m_nickBot + "\n";
	log[2] = "NICK " + m_nickBot + "\n";
	for (int i = 0; i < 3; ++i) {
		if (send(m_socketBot, log[i].c_str(), log[i].length(), SO_NOSIGPIPE) < 0) {
			std::cerr << "Error: send message!" << std::endl;
			exit(1);
		}
	}

	signal(SIGINT, sigHandler);
	while(work) {
		memset(m_recvBuf, 0, 1024);
		m_request = "";
		while ((m_recvBytes = recv(m_socketBot, m_recvBuf, 1024 - 1, 0)) > 0)
		{
			m_recvBuf[m_recvBytes] = '\0';
			m_request += m_recvBuf;
		}

		static std::string buf = "";

		while (!m_requestQueue.empty())
			m_requestQueue.pop();
		requestQueue(buf + m_request);
		while (m_requestQueue.size() > 0)
		{
			if (m_requestQueue.front().find('\n') != std::string::npos)
			{
				Request req(m_requestQueue.front());
				if (req.getCommand() == "PRIVMSG")
				{
					m_content = "";
					for (std::vector<std::string>::const_iterator it = req.getParams().begin() + 1; it !=
																									req.getParams().end(); ++it) {
						for (size_t i = 0; i < it->size(); ++i)
						{
							if (it->at(i) > 64 && it->at(i) < 91)
								m_content += (it->at(i) + 32);
							else if (it->at(i) > 96 && it->at(i) < 123)
								m_content += (it->at(i) - 32);
							else
								m_content += it->at(i);
						}
					}
					m_sender = ::getSender(req.getPrefix());
					std::string str = "PRIVMSG " + m_sender + " :" + m_content + "\n";
					if (send(m_socketBot, str.c_str(), str.length(), SO_NOSIGPIPE) < 0) {
						std::cerr << "Error: send message!" << std::endl;
						exit(1);
					}
					std::cout << "PRIVMSG " + m_sender + " :" + m_content + "\n";
				}
				else if (req.getCommand() == "PING")
				{
					std::string str = "PONG :" + req.getParams()[1];
					if (send(m_socketBot, str.c_str(), str.length(), SO_NOSIGPIPE) < 0)
					{
						std::cerr << "Error: send message!" << std::endl;
						exit(1);
					}
				}
			}
			else
				buf = m_requestQueue.front();
			m_requestQueue.pop();
		}
	}
}
