#include "../includes/Chat.hpp"

Chat::Chat(const std::string &chat_name, const Client &admin, const std::string &chat_password) :
		chatName(chat_name),
		m_passsword(chat_password),
		m_cLim(0),
		flags(NOMSGOUT) {
	m_users.push_back(&admin);
	m_adm.push_back(&admin);
	chatInfoVisual(admin);
}

Chat::~Chat() { }

void Chat::chatInfoVisual(const Client &member) {
	std::string text = member.getNick() + ": " + " joined!\n";
	for (const_iterator it = m_users.begin(); it != m_users.end(); ++it)
		if (*it != &member)
			(*it)->sendMessage(text);
	if (m_top.size() > 0)
		rplSend(IRC_SERV, member, 332, chatName, m_top);
	else
		rplSend(IRC_SERV, member, 331, chatName);
	std::string members_nicks;
	const Client* user;
	const_iterator it_begin = m_users.begin();
	const_iterator it_end = m_users.end();
	while(it_begin != it_end) {
		user = *it_begin;
		if (verifyAdmin(*user))
			members_nicks += "@";
		else if (verifySpeaker(*user))
			members_nicks += "+";
		members_nicks += user->getNick();
		++it_begin;
		if (it_begin != it_end)
			members_nicks += " ";
	}
	rplSend(IRC_SERV, member, 353, ": " + chatName, members_nicks);
	rplSend(IRC_SERV, member, 366, chatName);
}

bool Chat::verifyAdmin(const Client &user) const {
	for (size_t i = 0; i < m_adm.size(); ++i)
		if (m_adm[i]->getPrefix() == user.getPrefix())
			return (true);
	return (false);
}

bool Chat::verifySpeaker(const Client &user) const {
	for (size_t i = 0; i < m_speech.size(); ++i)
		if (m_speech[i]->getPrefix() == user.getPrefix())
			return (true);
	return (false);
}

bool Chat::isEmpty() const {
	if (m_users.size() == 0)
		return (true);
	return (false);
}

void Chat::sendMess(const std::string &message, const Client &sender) const {
	std::string text = sender.getNick() + ": " + message;
	for (const_iterator it = m_users.begin(); it != m_users.end(); ++it)
		if (*it != &sender)
			(*it)->sendMessage(text);
}

bool Chat::verifyInv(const Client &user) const {
	for (size_t i = 0; i < m_invUsers.size(); ++i)
		if (m_invUsers[i]->getUsername() == user.getPrefix())
			return (true);
	return (false);
}

bool Chat::verifyUsers(const std::string &nickname) const {
	for (const_iterator it = m_users.begin(); it != m_users.end(); ++it)
		if ((*it)->getNick() == nickname)
			return (true);
	return (false);
}

bool Chat::verifyUsersForBan(const std::string &mask, const std::string &prefix) {
	return verifyRgx(mask, prefix);
}

void Chat::connection(const Client &user, const std::string &pass) {
	if ((flags & PRIVATE) && pass != m_passsword)
		errSend(user, 475, chatName);
	else if (m_cLim != 0 && m_users.size() >= m_cLim)
		errSend(user, 471, chatName);
	else if ((flags & INVITEONLY) && !verifyInv(user))
		errSend(user, 473, chatName);
	else
	{
		for (size_t i = 0; i < m_ban.size(); i++) {
			if (verifyUsersForBan(m_ban[i], user.getPrefix())) {
				errSend(user, 474, chatName);
				return ;
			}
		}
		for (const_iterator it = m_users.begin(); it != m_users.end(); ++it)
			if ((*it)->getPrefix() == user.getPrefix())
				return ;
		m_users.push_back(&user);
		if (verifyInv(user))
		{
			size_t i;
			for (i = 0; i < m_invUsers.size(); ++i)
				if (m_invUsers[i] == &user)
					break;
			m_invUsers.erase(m_invUsers.begin() + i);
		}
		chatInfoVisual(user);
	}
}

const std::string& Chat::getChatName() const
{
	return (chatName);
}

const unsigned char& Chat::getFlags() const
{
	return (flags);
}

void Chat::invUsers(const Client &user, const Client &visiter)
{
	if (flags & INVITEONLY && !verifyAdmin(user))
		errSend(user, 482, chatName);
	else
	{
		m_invUsers.push_back(&visiter);
		visiter.sendMessage(":" + user.getPrefix() + " INVITE " + chatName + "\n");
		rplSend(IRC_SERV, user, 341, chatName, visiter.getNick());
		if (visiter.getFlags() & AWAY)
			rplSend(IRC_SERV, user, 301, visiter.getNick(),
					visiter.getExitMsg());
	}
}

std::string	Chat::strFlags() const
{
	std::string	string;
	if (flags & INVITEONLY)
		string += "i";
	if (flags & NOMSGOUT)
		string += "n";
	if (flags & PRIVATE)
		string += "p";
	if (flags & SECRET)
		string += "s";
	if (flags & TOPICSET)
		string += "t";
	return (string);
}


void Chat::disconnect(const Client& user)
{
	const_iterator it = m_users.begin();
	for (; it != m_users.end(); ++it)
		if (*it == &user)
			break ;
	m_users.erase(it);
	if (verifyAdmin(user))
	{
		size_t	i;
		for (i = 0; i < m_adm.size(); i++)
			if (m_adm[i] == &user)
				break;
		m_adm.erase(m_adm.begin() + i);
		if (m_adm.size() == 0 && m_users.size() > 0)
		{
			m_adm.push_back(m_users[0]);
			sendMess("MODE " + this->chatName + " +o " + m_users[0]->getNick() + "\n", user);
		}
	}
	if (verifySpeaker(user))
	{
		size_t	i;
		for (i = 0; i < m_speech.size(); i++)
			if (m_speech[i] == &user)
				break;
		m_speech.erase(m_speech.begin() + i);
	}
}

void Chat::chatInfoUsers(const Client &users)
{
	std::string	chat_name = "";
	std::string	info = "";
	if ((flags & SECRET) && !verifyUsers(users.getNick()))
		return ;
	if ((flags & PRIVATE) && !verifyUsers(users.getNick()))
		chat_name = "*";
	else
	{
		chat_name = chatName;
		info = "[+" + strFlags() + "] " + m_top;
	}
	std::stringstream	ss;
	ss << m_users.size();
	rplSend(IRC_SERV, users, 322, chat_name, ss.str(), info);
}