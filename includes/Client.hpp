#pragma once

#include "Message.hpp"

class Chat;

#define REGISTERED		0b00000001
#define RECEIVENOTICE	0b00000100
#define AWAY			0b00100000
#define PINGING			0b01000000
#define BREAKCONNECTION	0b10000000

class Client
{
private:
	typedef std::vector<const Chat *>::iterator iterator;

	int 						socketFd;
	std::string					m_pa;
	std::string					m_nick;
	std::string					realName;
	std::string					hostIp;
	time_t						registrationTime;
	time_t						timeLastMes;
	time_t						pingTiming;
	std::queue<std::string>		qmes;
	std::vector<const Chat*>	vecChats;
	std::string 				m_userName;
	std::string					exitMsg;
	unsigned char				flags;
	char						m_buff[100];
	int							m_recvB;

	Client();
	Client(const Client &ref);
	Client &operator=(const Client &ref);

	void messagesFilling(const std::string &str);

public:
	Client(int socket_fd, const std::string &ip);
	~Client();

	const int &getSocket() const;
	const std::string &getUsername() const;
	const std::string &getIpHost() const;
	const std::string &getNick() const;
	const std::string &getPassword() const;
	const time_t &getTimeLastMes() const;
	const time_t &getTimePing() const;
	const time_t &getRegistrationTime() const;
	const std::vector<const Chat*>& getChats() const;
	const std::queue<std::string>& getMessages() const;
	const unsigned char& getFlags() const;
	const std::string& getExitMsg() const;
	std::string getPrefix() const;

	void setUsername(const std::string &string);
	void setNick(const std::string &string);
	void setPassword(const std::string &string);
	void setExitMsg(const std::string &string);
	void setFlag(const unsigned char &flag);
	void setRealName(const std::string &string);

	void sendMessage(const std::string &string) const;
	bool verifyChat(const std::string &chat) const;
	int readMes();
	void deleteMes();
	void deleteFlag(unsigned char flag);
	void updateTimeLastMes();
	void updateTimePing();
	void addChat(const Chat &chat);
	void delChat(const std::string &chat);
};
