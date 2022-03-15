#include "../includes/utils.hpp"

int	send_reply(const std::string &from, const Client &user, int rpl, const std::string& arg1, const std::string& arg2, \
				 const std::string& arg3, const std::string& arg4, const std::string& arg5, const std::string& arg6, \
				 const std::string& arg7, const std::string& arg8)
{
	std::string msg = ":" + from + " ";
	std::stringstream ss;
	ss << rpl;
	msg += ss.str() + " " + user.get_nick() + " ";
	if (rpl == 301)
		msg += arg1 + " :" + arg2 + "\n";
	else if (rpl == 321)
		msg += "Channel :Users  Name\n";
	else if (rpl == 322)
		msg += arg1 + " " + arg2 + " :" + arg3 + "\n";
	else if (rpl == 323)
		msg += ":End of /LIST\n";
	else if (rpl == 331)
		msg += arg1 + " :No topic is set\n";
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
	send(user.get_socket(), msg.c_str(), msg.size(), SO_NOSIGPIPE);
	return 0;
}

int send_error(const Client &user, int err, const std::string &arg1, const std::string &arg2)
{
	std::string server_name = SERVER_NAME;
	std::string	msg = ":" + server_name + " ";
	std::stringstream ss;
	ss << err;
	msg += ss.str() + " " + server_name;
	if (err == 401)
		msg += " " + arg1 + " :No such nick/channel\n";
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
		msg += " " + arg1 + " :Not enough parameters\n";
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
	send(user.get_socket(), msg.c_str(), msg.size(), SO_NOSIGPIPE);
	return (-1);
}

bool	is_equal_to_regex(std::string mask, std::string str)
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

void log_message(const Message &msg)
{
	std::cout << std::endl << "m_prefix = " << msg.get_prefix() << ", m_command = " << msg.get_command();
	std::cout << ", paramsCount = " << msg.get_params().size() << std::endl;
	const std::vector<std::string>	params = msg.get_params();
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

bool is_valid_nick(const std::string &nick)
{
	if (nick.length() > 9)
		return (false);
	std::string	special = "-[]\\`^{}";
	for (size_t i = 0; i < nick.size(); i++)
	{
		if ((nick[i] >= 'a' && nick[i] <= 'z')
			|| (nick[i] >= 'A' && nick[i] <= 'Z')
			|| (nick[i] >= '0' && nick[i] <= '9')
			|| (special.find(nick[i]) != std::string::npos))
			continue ;
		else
			return (false);
	}
	return (true);
}