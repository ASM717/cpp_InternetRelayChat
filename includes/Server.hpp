#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include "utils.hpp"

#define BACK_LOG 20

class Server
{
private:
	typedef int (Server::*irc_methods)(const Message &, Client &);

	int 									m_port;
	std::string 							m_pass;
	int										m_timeout;
	static bool								boolWork;

	int										m_socket_fd;
	int										m_client_fd;
	sockaddr_in								m_addr;
	socklen_t								m_addrLen;
	std::vector<struct pollfd>				m_clientFds;
	char*									host_ip;
	Message *message;
	Client *client;

	std::map<std::string, irc_methods>		m_mapCmd;
//		std::map<Message &, Client &>		m_mapCmd;
	std::vector<Client*>					m_users;
	History									m_history;

	std::vector<std::string>				motd;

	std::map<std::string, std::string>		operators;
	std::map<std::string, Chat*>			chats;
	std::string								info;

	Server();
	Server(const Server& other);
	Server& operator=(const Server& other);
public:
	Server(int Port, std::string Password);
	~Server();

	void go();
private:
	static void sigProcess(int sig_code);
	bool isExistMember(const std::string &nick) const;
	void notify(Client& member, const std::string& msg);
	int connect_member(Client& member);
	std::queue<std::string>	fill(const std::string &str, char sep);
	bool is_exist_chat(const std::string& name) const;
	int handle_flags(const Message &msg, Client& member);
	bool is_correct_chat(const std::string& name);
	int connect_to_chat(const Client& member, const std::string& name, const std::string& password);
	void invite_to_chat(const Client& member, const std::string& nick, const std::string& chat_name);

	void init_cmds();

public:
	int handle_mes(Client& member);
	int pass_cmd(const Message &msg, Client &member);
	int nick_cmd(const Message &msg, Client &member);
	int user_cmd(const Message &msg, Client &member);
	int quit_cmd(const Message &msg, Client &member);
	int privmsg_cmd(const Message &msg, Client &member);
	int notice_cmd(const Message &msg, Client &member);
	int join_cmd(const Message &msg, Client &member);
	int invite_cmd(const Message &msg, Client &member);
	int kick_cmd(const Message &msg, Client &member);
	int part_cmd(const Message &msg, Client &member);
	int list_cmd(const Message &msg, Client &member);
	int pong_cmd(const Message &msg, Client &member);
	int ping_cmd(const Message &msg, Client &member);

	void send_motd(const Client& member) const;
	Client* get_member(const std::string& nick);
};

#endif
