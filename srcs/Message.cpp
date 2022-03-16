#include "../includes/Message.hpp"

Message::Message(const std::string &str)
{
	std::string	strWithoutNL = std::string(str.begin(), str.end() - 1);
	std::queue<std::string>	que = fullFill(strWithoutNL, ' ');

	if (que.size() > 0 && que.front()[0] == ':')
	{
		m_pref = std::string(que.front().begin() + 1, que.front().end());
		que.pop();
	}
	if (que.size() > 0)
	{
		m_cmd = que.front();
		que.pop();
	}
	while (que.size() > 0)
	{
		if (que.front()[0] == ':')
		{
			std::string	s(que.front().begin() + 1, que.front().end());
			que.pop();
			while (que.size() > 0)
			{
				s.append(" ");
				s.append(que.front());
				que.pop();
			}
			m_param.push_back(s);
		}
		else
		{
			m_param.push_back(que.front());
			que.pop();
		}
	}
}

Message::~Message()
{}

std::queue<std::string>	Message::fullFill(const std::string &string, char sep)
{
	std::queue<std::string>	ret;
	std::string::const_iterator	i = string.begin();
	while(i != string.end())
	{
		while (i != string.end() && *i == sep)
			++i;
		std::string::const_iterator	j = std::find(i, string.end(), sep);
		if (i != string.end())
		{
			ret.push(std::string(i, j));
			i = j;
		}
	}
	return ret;
}

const std::string	&Message::getPrefix() const
{
	return m_pref;
}

const std::string	&Message::getCommand() const
{
	return m_cmd;
}

const std::vector<std::string>	&Message::getParams() const
{
	return m_param;
}

int	rplSend(const std::string &from, const Client &user, int rpl, const std::string& arg1, const std::string& arg2, \
				 const std::string& arg3)
{
	std::string msg = ":" + from + " ";
	std::stringstream ss;
	ss << rpl;
	msg += ss.str() + " " + user.getNick() + " ";
	if (rpl == 301)
		msg += arg1 + " :" + arg2 + "\n";
	else if (rpl == 321)
		msg += "Channel :Users  Name\n";
	else if (rpl == 322)
		msg += arg1 + " " + arg2 + " :" + arg3 + "\n";
	else if (rpl == 323)
		msg += ":End of /LIST\n";
	else if (rpl == 331)
		msg += arg1 + " :No m_top is set\n";
	else if (rpl == 332)
		msg += arg1 + " :" + arg2 + "\n";
	else if (rpl == 341)
		msg += arg1 + " " + arg2 + "\n";
	else if (rpl == 353)
		msg += arg1 + " :" + arg2 + "\n";
	else if (rpl == 366)
		msg += arg1 + " :End of /NAMES list\n";
	else if (rpl == 372)
		msg += ":- " + arg1 + "\n";
	else if (rpl == 375)
		msg += ":- " + arg1 + " Message of the day - \n";
	else if (rpl == 376)
		msg += ":End of /MOTD m_command\n";
	else
		msg += "UNKNOWN REPLY\n";
	send(user.getSocket(), msg.c_str(), msg.size(), SO_NOSIGPIPE);
	return 0;
}

int errSend(const Client &user, int err, const std::string &arg1, const std::string &arg2)
{
	std::string server_name = IRC_SERV;
	std::string	msg = ":" + server_name + " ";
	std::stringstream ss;
	ss << err;
	msg += ss.str() + " " + server_name;
	if (err == 401)
		msg += " " + arg1 + " :No such m_nick/channel\n";
	else if (err == 402)
		msg += " " + arg1 + " :No such server\n";
	else if (err == 403)
		msg += " " + arg1 + " :No such channel\n";
	else if (err == 404)
		msg += " " + arg1 + " :Cannot send to channel\n";
	else if (err == 405)
		msg += " " + arg1 + " :You have joined too many channels\n";
	else if (err == 407)
		msg += " " + arg1 + " :Duplicate recipients. No arg1 delivered\n";
	else if (err == 409)
		msg += " :No origin specified\n";
	else if (err == 411)
		msg += " :No recipient given (" + arg1 + ")\n";
	else if (err == 412)
		msg += " :No text to send\n";
	else if (err == 421)
		msg += " " + arg1 + " :Unknown m_command\n";
	else if (err == 422)
		msg += " :MOTD File is missing\n";
	else if (err == 432)
		msg += " " + arg1 + " :Erroneus nickname\n";
	else if (err == 433)
		msg += " " + arg1 + " :Nickname is already in use\n";
	else if (err == 441)
		msg += " " + arg1 + " " + arg2 + " :They aren't on that channel\n";
	else if (err == 442)
		msg += " " + arg1 + " :You're not on that channel\n";
	else if (err == 443)
		msg += " " + arg1 + " " + arg2 + " :is already on channel\n";
	else if (err == 451)
		msg += " :You have not registered\n";
	else if (err == 461)
		msg += " " + arg1 + " :Not enough m_param\n";
	else if (err == 462)
		msg += " :You may not reregister\n";
	else if (err == 471)
		msg += " " + arg1 + " :Cannot join channel (+l)\n";
	else if (err == 473)
		msg += " " + arg1 + " :Cannot join channel (+i)\n";
	else if (err == 474)
		msg += " " + arg1 + " :Cannot join channel (+b)\n";
	else if (err == 475)
		msg += " " + arg1 + " :Cannot join channel (+k)\n";
	else if (err == 482)
		msg += " " + arg1 + " :You're not channel operator\n";
	else if (err == 501)
		msg += " :Unknown MODE flag\n";
	else
		msg += "UNKNOWN ERROR\n";
	send(user.getSocket(), msg.c_str(), msg.size(), SO_NOSIGPIPE);
	return (-1);
}

bool	verifyRgx(std::string mask, std::string str)
{
	const char *rs = 0;
	const char *rp;
	const char *s = str.c_str();
	const char *p = mask.c_str();
	while (1)
	{
		if (*p == '*')
		{
			rs = s;
			rp = ++p;
		}
		else if (!*s)
			return (!(*p));
		else if (*s == *p)
		{
			++s;
			++p;
		}
		else if (rs)
		{
			s = ++rs;
			p = rp;
		}
		else
			return false;
	}
}

void logging(const Message &message)
{
	std::cout << std::endl << "m_prefix = " << message.getPrefix() << ", m_command = " << message.getCommand();
	std::cout << ", paramsCount = " << message.getParams().size() << std::endl;
	const std::vector<std::string>	params = message.getParams();
	size_t	paramsSize = params.size();
	for (size_t i = 0; i < paramsSize; i++)
	{
		if (i == 0)
			std::cout << "Params list: \"" << params[i] << "\"";
		else
			std::cout << ", \"" << params[i] << "\"";
		if (i == (paramsSize - 1))
			std::cout << std::endl;
	}
	std::cout << std::endl;
}

bool verifyNickVal(const std::string &string)
{
	if (string.length() > 9)
		return (false);
	std::string	special = "-[]\\`^{}";
	for (size_t i = 0; i < string.size(); i++)
	{
		if ((string[i] >= 'a' && string[i] <= 'z')
			|| (string[i] >= 'A' && string[i] <= 'Z')
			|| (string[i] >= '0' && string[i] <= '9')
			|| (special.find(string[i]) != std::string::npos))
			continue ;
		else
			return (false);
	}
	return (true);
}