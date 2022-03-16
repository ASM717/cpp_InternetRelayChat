#include "../includes/Client.hpp"

Client::Client(int socket_fd, const std::string &ip) :
		socketFd(socket_fd),
		hostIp(ip),
		registrationTime(time(NULL)),
		flags(RECEIVENOTICE) { }

Client::~Client() { }

const int& Client::getSocket() const
{
	return (socketFd);
}

const std::string& Client::getUsername() const
{
	return (m_userName);
}

const std::string& Client::getIpHost() const
{
	return (hostIp);
}

const std::string& Client::getNick() const
{
	return (m_nick);
}

const std::string& Client::getPassword() const
{
	return (m_pa);
}

const time_t& Client::getTimeLastMes() const
{
	return (timeLastMes);
}

const time_t& Client::getTimePing() const
{
	return (pingTiming);
}

const std::queue<std::string> &Client::getMessages() const
{
	return (qmes);
}

const unsigned char &Client::getFlags() const
{
	return (flags);
}

const std::string &Client::getExitMsg() const
{
	return (exitMsg);
}

const time_t &Client::getRegistrationTime() const
{
	return (registrationTime);
}

const std::vector<const Chat*>& Client::getChats() const
{
	return (vecChats);
}

void Client::setUsername(const std::string &string)
{
	m_userName = string;
}

void Client::setNick(const std::string &string)
{
	m_nick = string;
}

void Client::setPassword(const std::string &string)
{
	m_pa = string;
}

void Client::setExitMsg(const std::string &string)
{
	exitMsg = string;
}

void Client::setFlag(const unsigned char &flag)
{
	flags |= flag;
	if (flag == BREAKCONNECTION && exitMsg.size() == 0)
		exitMsg = "сlient exited";
}

void Client::sendMessage(const std::string &string) const
{
	if (string.size() > 0)
		send(socketFd, string.c_str(), string.size(), SO_NOSIGPIPE);
}

void Client::messagesFilling(const std::string &str)
{
	typedef std::string::const_iterator const_iterator;
	for (const_iterator it = str.begin(); it != str.end(); ++it)
	{
		for (; it != str.end() && *it == '\n'; ++it)
			;
		const_iterator separator = std::find(it, str.end(), '\n');
		if (it != str.end())
		{
			qmes.push(std::string(it, separator + 1));
			it = separator;
		}
	}
}

int Client::readMes()
{
	std::string text;

	if (qmes.size() > 0)
		text = qmes.front();
	while((m_recvB = recv(socketFd, m_buff, 201, 0)) > 0)
	{
		m_buff[m_recvB] = '\0';
		text += m_buff;
		m_buff[0] = 0;
		if (text.find('\n') != std::string::npos)
			break;
	}
	if (text.length() > 512)
		text = text.substr(0, 512 - 2) + "\r\n";
	if (m_recvB == 0)
		return (-1);
	while (text.find("\r\n") != std::string::npos)
		text.replace(text.find("\r\n"), 2, "\n");
	if (text.size() > 1)
		messagesFilling(text);
	return 0;
}

void Client::setRealName(const std::string &string)
{
	realName = string;
}

std::string Client::getPrefix() const
{
	return std::string(m_nick + "!" + m_userName + "@" + realName);
}

void Client::deleteMes()
{
	if (qmes.size() > 0)
		qmes.pop();
}

void Client::deleteFlag(unsigned char flag)
{
	flags &= ~flag;
}

void Client::updateTimeLastMes()
{
	timeLastMes = time(0);
}

void Client::updateTimePing()
{
	pingTiming = time(0);
}

void Client::addChat(const Chat &chat)
{
	vecChats.push_back(&chat);
}

bool Client::verifyChat(const std::string &chat) const
{
	for (size_t i = 0; i < vecChats.size(); i++)
		if (vecChats[i]->getChatName() == chat)
			return true;
	return false;
}

void	Client::delChat(const std::string &chat)
{
	iterator begin = vecChats.begin();
	iterator end = vecChats.end();
	for (; begin != end; ++begin)
		if ((*begin)->getChatName() == chat)
			break ;
	vecChats.erase(begin);
}
