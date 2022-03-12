#include "utils.hpp"

void exit_program(const std::string &error_msg)
{
	std::cerr << RED << error_msg << RESET << std::endl;
	exit(EXIT_FAILURE);
}

//int	check_input_params(int argc, char** argv)
//{
//	int 	m_port;
//
//	if (argc != 3)
//		exit_program("Usage: ./ircserv <m_port> <m_pass>");
//	m_port = atoi(argv[1]);
//	if (m_port < 1024 || m_port > 65535)
//		exit_program("Wrong m_port!");
//	return m_port;
//}

int		send_reply(const std::string &from, const Client &user, int rpl, const std::string& arg1, const std::string& arg2, \
				 const std::string& arg3, const std::string& arg4, const std::string& arg5, const std::string& arg6, \
				 const std::string& arg7, const std::string& arg8)
{
	std::string	msg = ":" + from + " ";
	std::stringstream	ss;
	ss << rpl;
	msg += ss.str() + " " + user.get_nick() + " ";
	switch (rpl)
	{
		case 302:
			msg += ":" + arg1 + "\n";
			break;
		case 303:
			msg += ":" + arg1 + "\n";
			break;
		case 301:
			msg += arg1 + " :" + arg2 + "\n";
			break;
		case 305:
			msg += ":You are no longer marked as being away\n";
			break;
		case 306:
			msg += ":You have been marked as being away\n";
			break;
		case 311:
			msg += arg1 + " " + arg2 + " " + arg3 + " * :" + arg4 + "\n";
			break;
		case 312:
			msg += arg1 + " " + arg2 + " :" + arg3 + "\n";
			break;
		case 313:
			msg += arg1 + " :is an IRC operator\n";
			break;
		case 317:
			msg += arg1 + " " + arg2 + " " + arg3 + " :seconds idle\n";
			break;
		case 318:
			msg += arg1 + " :End of /WHOIS list\n";
			break;
		case 319:
			msg += arg1 + " :" + arg2 + "\n";
			break;
		case 314:
			msg += arg1 + " " + arg2 + " " + arg3 + " * :" + arg4 + "\n";
			break;
		case 369:
			msg += arg1 + " :End of WHOWAS\n";
			break;
		case 321:
			msg += "Channel :Users  Name\n";
			break;
		case 322:
			msg += arg1 +  " " + arg2 + " :" + arg3 + "\n";
			break;
		case 323:
			msg += ":End of /LIST\n";
			break;
		case 324:
			msg += arg1 + " +" + arg2 + "\n";
			break;
		case 331:
			msg += arg1 + " :No topic is set\n";
			break;
		case 332:
			msg += arg1 + " :" + arg2 + "\n";
			break;
		case 341:
			msg += arg1 + " " + arg2 + "\n";
			break;
		case 342:
			msg += arg1 + " :Summoning user to IRC\n";
			break;
		case 351:
			msg += arg1 + "." + arg2 + " " + arg3 + " :" + arg4 + "\n";
			break;
		case 352:
			msg += arg1 + " " + arg2 + " " + arg3 + " " + arg4 + " ";
			msg += arg5 + " " + arg6 + " :" + arg7 + " " + arg8 + "\n";
			break;
		case 315:
			msg += arg1 + " :End of /WHO list\n";
			break;
		case 353:
			msg += arg1 + " :" + arg2 + "\n";
			break;
		case 366:
			msg += arg1 + " :End of /NAMES list\n";
			break;
		case 364:
			msg += arg1 + " " + arg2 + ": " + arg3 + " " + arg4 + "\n";
			break;
		case 365:
			msg += arg1 + " :End of /LINKS list\n";
			break;
		case 367:
			msg += arg1 + " " + arg2 + "\n";
			break;
		case 368:
			msg += arg1 + " :End of channel ban list\n";
			break;
		case 371:
			msg += ":" + arg1 + "\n";
			break;
		case 374:
			msg += ":End of /INFO list\n";
			break;
		case 375:
			msg += ":- " + arg1 + " Message of the day - \n";
			break;
		case 372:
			msg += ":- " + arg1 + "\n";
			break;
		case 376:
			msg += ":End of /MOTD command\n";
			break;
		case 381:
			msg += ":You are now an IRC operator\n";
			break;
		case 382:
			msg += arg1 + " :Rehashing\n";
			break;
		case 391:
			msg += arg1 + " :" + arg2;
			break;
		case 392:
			msg += ":UserID   Terminal  Host\n";
			break;
		case 393:
			msg += ":%-8s %-9s %-8s\n";
			break;
		case 394:
			msg += ":End of users\n";
			break;
		case 395:
			msg += ":Nobody logged in\n";
			break;
		case 200:
			msg += "Link " + arg1 + " " + arg2 + " " + arg3 + "\n";
			break;
		case 201:
			msg += "Try. " + arg1 + " " + arg2 + "\n";
			break;
		case 202:
			msg += "H.S. " + arg1 + " " + arg2 + "\n";
			break;
		case 203:
			msg += "???? " + arg1 + " " + arg2 + "\n";
			break;
		case 204:
			msg += "Oper " + arg1 + " " + arg2 + "\n";
			break;
		case 205:
			msg += "User " + arg1 + " " + arg2 + "\n";
			break;
		case 206:
			msg += "Serv " + arg1 + " " + arg2 + "S " + arg3 + "C ";
			msg += arg4 + " " + arg5 + "@" + arg6 + "\n";
			break;
		case 208:
			msg += arg1 + " 0 " + arg2 + "\n";
			break;
		case 261:
			msg += "File " + arg1 + " " + arg2 + "\n";
			break;
		case 211:
			msg += arg1 + " " + arg2 + " " + arg3 + " " + arg4 + " ";
			msg += arg5 + " " + arg6 + " " + arg7 + "\n";
			break;
		case 212:
			msg += arg1 + " " + arg2 + "\n";
			break;
		case 213:
			msg += "C " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4 + "\n";
			break;
		case 214:
			msg += "N " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4 + "\n";
			break;
		case 215:
			msg += "I " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4 + "\n";
			break;
		case 216:
			msg += "K " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4 + "\n";
			break;
		case 218:
			msg += "Y " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4 + "\n";
			break;
		case 219:
			msg += arg1 + " :End of /STATS report\n";
			break;
		case 241:
			msg += "L " + arg1 + " * " + arg2 + " " + arg3 + "\n";
			break;
		case 242:
			msg += ":Server Up %d days %d:%02d:%02d\n";
			break;
		case 243:
			msg += "O " + arg1 + " * " + arg2 + "\n";
			break;
		case 244:
			msg += "H " + arg1 + " * " + arg2 + "\n";
			break;
		case 221:
			msg += arg1 + "\n";
			break;
		case 251:
			msg += ":There are " + arg1 + " users and " + arg2;
			msg += " invisible on " + arg3 + " servers\n";
			break;
		case 252:
			msg += arg1 + " :operator(s) online\n";
			break;
		case 253:
			msg += arg1 + " :unknown connection(s)\n";
			break;
		case 254:
			msg += arg1 + " :channels formed\n";
			break;
		case 255:
			msg += ":I have " + arg1 + " clients and " + arg2 + " servers\n";
			break;
		case 256:
			msg += arg1 + " :Administrative info\n";
			break;
		case 257:
			msg += ":Name     " + arg1 + "\n";
			break;
		case 258:
			msg += ":Nickname " + arg1 + "\n";
			break;
		case 259:
			msg += ":E-Mail   " + arg1 + "\n";
			break;
		default:
			msg += "UNKNOWN REPLY\n";
			break;
	}
	send(user.get_socket(), msg.c_str(), msg.size(), SO_NOSIGPIPE);
	return 0;
}

int		send_error(const Client &user, int err, const std::string &arg1, const std::string &arg2)
{
	std::string server_name = SERVER_NAME;
	std::string	msg = ":" + server_name + " ";
	std::stringstream ss;
	ss << err;
	msg += ss.str() + " " + server_name;
	switch (err)
	{
		case 401:
			msg += " " + arg1 + " :No such nick/channel\n";
			break;
		case 402:
			msg += " " + arg1 + " :No such server\n";
			break;
		case 403:
			msg += " " + arg1 + " :No such channel\n";
			break;
		case 404:
			msg += " " + arg1 + " :Cannot send to channel\n";
			break;
		case 405:
			msg += " " + arg1 + " :You have joined too many channels\n";
			break;
		case 406:
			msg += " " + arg1 + " :There was no such nickname\n";
			break;
		case 407:
			msg += " " + arg1 + " :Duplicate recipients. No arg1 delivered\n";
			break;
		case 409:
			msg += " :No origin specified\n";
			break;
		case 411:
			msg += " :No recipient given (" + arg1 + ")\n";
			break;
		case 412:
			msg += " :No text to send\n";
			break;
		case 413:
			msg += " " + arg1 + " :No toplevel domain specified\n";
			break;
		case 414:
			msg += " " + arg1 + " :Wildcard in toplevel domain\n";
			break;
		case 421:
			msg += " " + arg1 + " :Unknown command\n";
			break;
		case 422:
			msg += " :MOTD File is missing\n";
			break;
		case 423:
			msg += " " + arg1 + " :No administrative info available\n";
			break;
		case 424:
			msg += " :File error doing \n" + arg1 + " on " + arg2 + "\n";
			break;
		case 431:
			msg += " :No nickname given\n";
			break;
		case 432:
			msg += " " + arg1 + " :Erroneus nickname\n";
			break;
		case 433:
			msg += " " + arg1 + " :Nickname is already in use\n";
			break;
		case 436:
			msg += " " + arg1 + " :Nickname collision KILL\n";
			break;
		case 441:
			msg += " " + arg1 + " " + arg2 + " :They aren't on that channel\n";
			break;
		case 442:
			msg += " " + arg1 + " :You're not on that channel\n";
			break;
		case 443:
			msg += " " + arg1 + " " + arg2 + " :is already on channel\n";
			break;
		case 444:
			msg += " " + arg1 + " :User not logged in\n";
			break;
		case 445:
			msg += " :SUMMON has been disabled\n";
			break;
		case 446:
			msg += " :USERS has been disabled\n";
			break;
		case 451:
			msg += " :You have not registered\n";
			break;
		case 461:
			msg += " " + arg1 + " :Not enough parameters\n";
			break;
		case 462:
			msg += " :You may not reregister\n";
			break;
		case 463:
			msg += " :Your host isn't among the privileged\n";
			break;
		case 464:
			msg += " :Password incorrect\n";
			break;
		case 465:
			msg += " :You are banned from this server\n";
			break;
		case 467:
			msg += " " + arg1 + " :Channel key already set\n";
			break;
		case 471:
			msg += " " + arg1 + " :Cannot join channel (+l)\n";
			break;
		case 472:
			msg += " " + arg1 + " :is unknown mode char to me\n";
			break;
		case 473:
			msg += " " + arg1 + " :Cannot join channel (+i)\n";
			break;
		case 474:
			msg += " " + arg1 + " :Cannot join channel (+b)\n";
			break;
		case 475:
			msg += " " + arg1 + " :Cannot join channel (+k)\n";
			break;
		case 481:
			msg += " :Permission Denied- You're not an IRC operator\n";
			break;
		case 482:
			msg += " " + arg1 + " :You're not channel operator\n";
			break;
		case 483:
			msg += " :You cant kill a server!\n";
			break;
		case 491:
			msg += " :No O-lines for your host\n";
			break;
		case 501:
			msg += " :Unknown MODE flag\n";
			break;
		case 502:
			msg += " :Cant change mode for other users\n";
			break;
		default:
			msg += "UNKNOWN ERROR\n";
			break;
	}
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

void	log_message(const Message &msg)
{
	std::cout << std::endl << "prefix = " << msg.get_prefix() << ", command = " << msg.get_command();
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

bool	is_valid_nick(const std::string &nick)
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
