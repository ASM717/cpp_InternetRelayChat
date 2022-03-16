#pragma once
#include "Message.hpp"

class Server {
private:
	typedef int (Server::*ft_methods)(const Message &, Client &);

	int 									m_port;
	std::string 							m_pass;
	int										m_timeout;
	static bool								boolWork;
	int										m_socket_fd;
	int										m_client_fd;
	sockaddr_in								m_addr;
	socklen_t								m_addrLen;
	std::vector<struct pollfd>				m_clientFds;
	std::map<std::string, ft_methods>		m_mapCmd;
	std::vector<Client*>					m_users;
	Hist									m_history;
	std::vector<std::string>				motd;
	std::map<std::string, Chat*>			chats;

	Server();
	Server(const Server& other);
	Server& operator=(const Server& other);

	static void sigProcess(int sig_code);
	bool verifyExistUsers(const std::string &string) const;
	void notification(Client &client, const std::string &string);
	int usersConnection(Client &cl);
	std::queue<std::string>	qFiller(const std::string &string, char sep);
	bool verifyExtChat(const std::string &name) const;
	bool verifyCorrectionChat(const std::string &name);
	int chatConnectionUs(const Client &users, const std::string &name, const std::string &password);
	void invChat(const Client &user, const std::string &nick, const std::string &chat_name);
	void allIRCCommands();

public:
	Server(int Port, std::string Password);
	~Server();

	void go();
	int handleMes(Client& member);
	int cmdPassword(const Message &msg, Client &member);
	int cmdNickName(const Message &msg, Client &member);
	int cmdUser(const Message &msg, Client &member);
	int cmdInv(const Message &msg, Client &member);
	int cmdKick(const Message &msg, Client &member);
	int cmdPart(const Message &msg, Client &member);
	int cmdList(const Message &msg, Client &member);
	int cmdPong(const Message &msg, Client &member);
	int cmdQuit(const Message &msg, Client &member);
	int cmdPrivMessage(const Message &msg, Client &member);
	int cmdNotice(const Message &msg, Client &member);
	int cmdJoin(const Message &msg, Client &member);
	int cmdPing(const Message &msg, Client &member);
	void putMotd(const Client& member) const;
	Client* getMembers(const std::string& nick);
};

