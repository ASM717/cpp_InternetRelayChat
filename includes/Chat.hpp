#pragma once
#include "Message.hpp"

class Client;

#define PRIVATE		0b000001
#define SECRET		0b000010
#define MODERATED	0b000100
#define INVITEONLY	0b001000
#define TOPICSET	0b010000
#define NOMSGOUT	0b100000

class Chat {
private:
	typedef std::vector<const Client*>::const_iterator const_iterator;

	std::string					chatName;
	std::string					m_top;
	std::vector<const Client*>	m_adm;
	std::vector<const Client*>	m_speech;
	std::vector<const Client*>	m_users;
	std::vector<const Client*>	m_invUsers;
	std::vector<std::string>	m_ban;
	std::string					m_passsword;
	unsigned int				m_cLim;
	unsigned char				flags;

	Chat();
	Chat(const Chat& other);
	Chat& operator=(const Chat& other);

	void chatInfoVisual(const Client &member);
public:
	Chat(const std::string &chat_name, const Client &admin, const std::string &chat_password = "");
	virtual ~Chat();
	void sendMess(const std::string& message, const Client& sender) const;
	const std::string &getChatName() const;
	const unsigned char &getFlags() const;
	bool verifyAdmin(const Client &user) const;
	bool verifySpeaker(const Client &user) const;
	bool isEmpty() const;
	bool verifyInv(const Client &user) const;
	bool verifyUsers(const std::string &nickname) const;
	bool verifyUsersForBan(const std::string &mask, const std::string &prefix);
	void connection(const Client &user, const std::string &pass);
	void invUsers(const Client &user, const Client &visiter);
	void disconnect(const Client &user);
	void chatInfoUsers(const Client &users);
	std::string strFlags() const;
};

