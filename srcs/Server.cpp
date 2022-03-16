#include "../includes/Server.hpp"

bool Server::boolWork;

Server::Server(int Port, std::string Password): m_port(Port), m_pass(Password), m_timeout(1)
{
	std::cout << "IRCserver is started at m_port " << m_port << std::endl;
	allIRCCommands();
	std::string line = "Hello man, you are connection";
	motd.push_back(line);
}

Server::~Server()
{
	for (size_t i = 0; i < m_users.size(); ++i)
	{
		close(m_users[i]->getSocket());
		delete m_users[i];
	}
	for (std::map<std::string, Chat *>::const_iterator it = chats.begin(); it != chats.end(); ++it)
		delete it->second;
	close(m_socket_fd);
}

int Server::handleMes(Client& member)
{
	while (member.getMessages().size() > 0 &&
			member.getMessages().front()[member.getMessages().front().size() - 1] == '\n')
	{
		Message	msg(member.getMessages().front());
		member.deleteMes();
		logging(msg);
		if (!(member.getFlags() & REGISTERED) && msg.getCommand() != "QUIT" && msg.getCommand() != "PASS" \
 && msg.getCommand() != "USER" && msg.getCommand() != "NICK")
			errSend(member, 451);
		else {
			try {
				int ret = (this->*(m_mapCmd.at(msg.getCommand())))(msg, member);
				if (ret == -1)
					return (-1);
			}
			catch(const std::exception& e) {
				errSend(member, 421, msg.getCommand());
			}
		}
	}
	member.updateTimeLastMes();
	return (0);
}

int Server::cmdPassword(const Message &msg, Client &member)
{
	if (msg.getParams().size() == 0)
		errSend(member, 461, msg.getCommand());
	else if (member.getFlags() & REGISTERED)
		errSend(member, 462);
	else
		member.setPassword(msg.getParams()[0]);
	return (0);
}

int Server::cmdNickName(const Message &msg, Client &member)
{
	if (msg.getParams().size() == 0)
		errSend(member, 461, msg.getCommand());
	else if (!verifyNickVal(msg.getParams()[0]) || msg.getParams()[0] == IRC_SERV)
		errSend(member, 432, msg.getParams()[0]);
	else if (verifyExistUsers(msg.getParams()[0]))
		errSend(member, 433, msg.getParams()[0]);
	else {
		if (member.getFlags() & REGISTERED) {
			notification(member, ":" + member.getPrefix() + " " + msg.getCommand() + " " + msg.getParams()[0] + "\n");
			m_history.addUser(member);
		}
		member.setNick(msg.getParams()[0]);
	}
	return usersConnection(member);
}

int Server::cmdUser(const Message &msg, Client &member)
{
	if (msg.getParams().size() < 4)
		errSend(member, 461, msg.getCommand());
	else if (member.getFlags() & REGISTERED)
		errSend(member, 462);
	else {
		member.setUsername(msg.getParams()[0]);
		member.setRealName(msg.getParams()[3]);
	}
	return usersConnection(member);
}

int Server::cmdQuit(const Message &msg, Client &member)
{
	if (msg.getParams().size() > 0)
		member.setExitMsg(msg.getParams()[0]);
	m_history.addUser(member);
	return (-1);
}

int Server::cmdPrivMessage(const Message &msg, Client &member)
{
	if (msg.getParams().size() == 0)
		return (errSend(member, 411, msg.getCommand()));
	if (msg.getParams().size() == 1)
		return (errSend(member, 412));

	std::queue<std::string> receivers = qFiller(msg.getParams()[0], ',');
	std::set<std::string> uniq;

	if (msg.getCommand() == "NOTICE" && (receivers.size() > 1 || receivers.front()[0] == '#' || receivers.front()[0] == '&'))
		return (errSend(member, 401, msg.getParams()[0]));

	while (receivers.size() > 0) {
		if (uniq.find(receivers.front()) != uniq.end())
			return (errSend(member, 407, receivers.front()));
		if (receivers.front()[0] == '#' || receivers.front()[0] == '&') {
			if (!verifyExtChat(receivers.front()))
				return (errSend(member, 401, receivers.front()));
			if (!chats[receivers.front()]->verifyUsers(member.getNick()))
				return (errSend(member, 404, receivers.front()));
		} else if (!verifyExistUsers(receivers.front()))
			return (errSend(member, 401, msg.getParams()[0]));
		uniq.insert(receivers.front());
		receivers.pop();
	}

	for (std::set<std::string>::iterator it = uniq.begin(); it != uniq.end(); ++it) {
		if ((*it)[0] == '#' || (*it)[0] == '&') {
			Chat *receiver_chat = chats[*it];
			if (receiver_chat->getFlags() & MODERATED && (!receiver_chat->verifyAdmin(member)
														  && !receiver_chat->verifySpeaker(member)))
				errSend(member, 404, *it);
			else
				receiver_chat->sendMess(msg.getCommand() + " " + *it + " :"
										+ msg.getParams()[1] + "\n", member);
		} else {
			if (msg.getCommand() == "PRIVMSG" && (getMembers(*it)->getFlags() & AWAY))
				rplSend(IRC_SERV, member, 301, *it, getMembers(*it)->getExitMsg());
			if (msg.getCommand() != "NOTICE" || (getMembers(*it)->getFlags() & RECEIVENOTICE))
				getMembers(*it)->sendMessage(":" + member.getPrefix() + " "
											 + msg.getCommand() + " " + *it + " :" + msg.getParams()[1] + "\n");
		}
	}
	return (0);
}

int Server::cmdNotice(const Message &msg, Client &member) {
	cmdPrivMessage(msg, member);
	return (0);
}

int Server::cmdJoin(const Message &msg, Client &member)
{
	if (msg.getParams().size() == 0)
		errSend(member, 461, msg.getCommand());
	else {
		std::queue<std::string>	chats_queue = qFiller(msg.getParams()[0], ',');
		std::queue<std::string>	keys;
		if (msg.getParams().size() > 1)
			keys = qFiller(msg.getParams()[1], ',');
		for (; chats_queue.size() > 0; chats_queue.pop()) {
			std::string	key = keys.size() ? keys.front() : "";
			if (keys.size() > 0)
				keys.pop();
			if (!verifyCorrectionChat(chats_queue.front()))
				errSend(member, 403, chats_queue.front());
			else if (member.getChats().size() >= 10)
				errSend(member, 405, chats_queue.front());
			else if (chatConnectionUs(member, chats_queue.front(), key) == 1)
				member.addChat(*(chats.at(chats_queue.front())));
		}
	}
	return (0);
}

int Server::cmdInv(const Message &msg, Client &member) {
	if (msg.getParams().size() < 2)
		errSend(member, 461, "INVITE");
	else if (!verifyExistUsers(msg.getParams()[0]))
		errSend(member, 401, msg.getParams()[0]);
	else if (!member.verifyChat(msg.getParams()[1]) || !verifyExtChat(msg.getParams()[1]))
		errSend(member, 442, msg.getParams()[1]);
	else
		invChat(member, msg.getParams()[0], msg.getParams()[1]);
	return (0);
}

int Server::cmdKick(const Message &msg, Client &member) {
	if (msg.getParams().size() < 2)
		errSend(member, 461, "KICK");
	else if (!verifyExtChat(msg.getParams()[0]))
		errSend(member, 403, msg.getParams()[0]);
	else if (!chats.at(msg.getParams()[0])->verifyAdmin(member))
		errSend(member, 482, msg.getParams()[0]);
	else if (!chats.at(msg.getParams()[0])->verifyUsers(member.getNick()))
		errSend(member, 442, msg.getParams()[0]);
	else if (!verifyExistUsers(msg.getParams()[1]))
		errSend(member, 401, msg.getParams()[1]);
	else if (!chats.at(msg.getParams()[0])->verifyUsers(msg.getParams()[1]))
		errSend(member, 441, msg.getParams()[1], msg.getParams()[0]);
	else {
		Chat *chat = chats.at(msg.getParams()[0]);
		std::string	message = "KICK " + chat->getChatName() + " " + msg.getParams()[1] + " :";
		if (msg.getParams().size() > 2)
			message += msg.getParams()[2];
		else
			message += member.getNick();
		chat->sendMess(message + "\n", member);
		chat->disconnect(*(getMembers(msg.getParams()[1])));
		getMembers(msg.getParams()[1])->delChat(msg.getParams()[0]);
	}
	return (0);
}

int Server::cmdPart(const Message &msg, Client &member) {
	if (msg.getParams().size() < 1)
		errSend(member, 461, "PART");
	else {
		std::queue<std::string>	chat_queue = qFiller(msg.getParams()[0], ',');
		while (chat_queue.size() > 0) {
			if (!verifyExtChat(chat_queue.front()))
				errSend(member, 403, chat_queue.front());
			else if (!member.verifyChat(chat_queue.front()))
				errSend(member, 442, chat_queue.front());
			else {
				chats.at(chat_queue.front())->sendMess("PART " + chat_queue.front() + "\n", member);
				chats.at(chat_queue.front())->disconnect(member);
				member.delChat(chat_queue.front());
			}
			chat_queue.pop();
		}
	}
	return (0);
}

int Server::cmdList(const Message &msg, Client &member) {
	if (msg.getParams().size() > 1 && msg.getParams()[1] != IRC_SERV)
		return (errSend(member, 402, msg.getParams()[1]));
	std::queue<std::string>	chans;
	std::vector<std::string>	chansToDisplay;
	if (msg.getParams().size() > 0)
	{
		for (chans = qFiller(msg.getParams()[0], ','); chans.size() > 0; ) {
			if (verifyExtChat(chans.front()))
				chansToDisplay.push_back(chans.front());
			chans.pop();
		}
	}
	else
	{
		for (std::map<std::string, Chat*>::const_iterator it = chats.begin(); it != chats.end(); ++it)
			chansToDisplay.push_back((*it).first);
	}
	rplSend(IRC_SERV, member, 321);
	for (size_t i = 0; i < chansToDisplay.size(); ++i)
		chats.at(chansToDisplay[i])->chatInfoUsers(member);
	rplSend(IRC_SERV, member, 323);
	return (0);
}

void Server::sigProcess(int sig_code)
{
	(void)sig_code;
	boolWork = false;
}

bool Server::verifyExistUsers(const std::string &string) const
{
	size_t	usersCount = m_users.size();
	for (size_t i = 0; i < usersCount; i++)
	{
		if (m_users[i]->getNick() == string)
			return (true);
	}
	return (false);
}

void Server::notification(Client &client, const std::string &string)
{
	const std::vector<const Chat *> chans = client.getChats();
	for (size_t i = 0; i < m_users.size(); i++)
	{
		for (size_t j = 0; j < chans.size(); j++)
		{
			if (chans[j]->verifyUsers(m_users[i]->getNick()))
			{
				m_users[i]->sendMessage(string);
				break ;
			}
		}
	}
}

int Server::usersConnection(Client &cl)
{
	if (cl.getNick().size() > 0 && cl.getUsername().size() > 0)
	{
		if (m_pass.size() == 0 || cl.getPassword() == m_pass)
		{
			if (!(cl.getFlags() & REGISTERED))
			{
				cl.setFlag(REGISTERED);
				putMotd(cl);
			}
		}
		else
			return (-1);
	}
	return (0);
}

void Server::putMotd(const Client &member) const
{
	if (motd.size() == 0)
		errSend(member, 422);
	else
	{
		rplSend(IRC_SERV, member, 375, IRC_SERV);
		for (size_t i = 0; i < motd.size(); ++i)
			rplSend(IRC_SERV, member, 372, motd[i]);
		rplSend(IRC_SERV, member, 376);
	}
}

std::queue<std::string>	Server::qFiller(const std::string &string, char sep)
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
	return (ret);
}

bool Server::verifyExtChat(const std::string &name) const
{
	try {
		chats.at(name);
		return (true);
	}
	catch(const std::exception& e)
	{}
	return (false);
}

Client* Server::getMembers(const std::string &nick)
{
	Client	*ret;

	for (size_t i = 0; i < m_users.size(); i++)
		if (m_users[i]->getNick() == nick)
			ret = m_users[i];
	return ret;
}

bool Server::verifyCorrectionChat(const std::string &name)
{
	if (name[0] != '#' && name[0] != '&')
		return (false);
	for (size_t i = 1; i < name.size(); i++)
	{
		if (name[i] == ' ' || name[i] == 7 || name[i] == 0 \
			|| name[i] == 13 || name[i] == 10 || name[i] == ',')
			return (false);
	}
	return (true);
}

int Server::chatConnectionUs(const Client& users, const std::string& name, const std::string& password)
{
	try
	{
		Chat *tmp = chats.at(name);
		tmp->connection(users, password);
		return (1);
	}
	catch(const std::exception& e)
	{
		chats[name] = new Chat(name, users, password);
	}
	return (1);
}

void Server::invChat(const Client &user, const std::string &nick, const std::string &chat_name)
{
	Client	*receiver;
	for (size_t i = 0; i < m_users.size(); ++i)
		if (m_users[i]->getNick() == nick)
			receiver = m_users[i];
	Chat *chat = chats.at(chat_name);
	if (chat->verifyUsers(nick))
		errSend(user, 443, nick, chat_name);
	else
		chat->invUsers(user, *receiver);
}

int Server::cmdPing(const Message &msg, Client &member)
{
	std::string name = IRC_SERV;
	if (msg.getParams().size() == 0)
		return (errSend(member, 409));
	member.sendMessage(":" + name + " PONG :" + msg.getParams()[0] + "\n");
	return 0;
}

int Server::cmdPong(const Message &msg, Client &member)
{
	if (msg.getParams().size() <= 0 || msg.getParams()[0] != IRC_SERV)
		return (errSend(member, 402, msg.getParams().size() > 0 ? msg.getParams()[0] : ""));
	member.deleteFlag(PINGING);
	return 0;
}

void Server::allIRCCommands()
{
	m_mapCmd["PASS"] = &Server::cmdPassword;
	m_mapCmd["NICK"] = &Server::cmdNickName;
	m_mapCmd["USER"] = &Server::cmdUser;
	m_mapCmd["QUIT"] = &Server::cmdQuit;
	m_mapCmd["PRIVMSG"] = &Server::cmdPrivMessage;
	m_mapCmd["NOTICE"] = &Server::cmdNotice;
	m_mapCmd["JOIN"] = &Server::cmdJoin;
	m_mapCmd["INVITE"] = &Server::cmdInv;
	m_mapCmd["KICK"] = &Server::cmdKick;
	m_mapCmd["PART"] = &Server::cmdPart;
	m_mapCmd["LIST"] = &Server::cmdList;
	m_mapCmd["PING"] = &Server::cmdPing;
	m_mapCmd["PONG"] = &Server::cmdPong;
}

void Server::go() {
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(m_port);
	m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(m_addr.sin_zero, 0, sizeof(m_addr.sin_zero));
	m_addrLen = sizeof(m_addr);
	boolWork = true;

	signal(SIGINT, sigProcess);
	if ((m_socket_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "Error: Socket initialization error!" << std::endl;
		exit(1);
	}
	int	yes = 1;
	if (setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1
		|| bind(m_socket_fd, reinterpret_cast<struct sockaddr *>(&m_addr), m_addrLen) == -1) {
		std::cerr << "Error: setsockopt failed!" << std::endl;
		exit(1);
	}

	if (listen(m_socket_fd, 10) == -1) {
		std::cerr << "Error: listen failed!" << std::endl;
		exit(1);
	}
	fcntl(m_socket_fd, F_SETFL, O_NONBLOCK);

	while(boolWork) {
		if ((m_client_fd = accept(m_socket_fd, reinterpret_cast<struct sockaddr*>(&m_addr), &m_addrLen)) >= 0) {
			struct pollfd poll_fd;
			poll_fd.fd = m_client_fd;
			poll_fd.events = POLLIN;
			poll_fd.revents = 0;
			m_clientFds.push_back(poll_fd);
			m_users.push_back(new Client(m_client_fd, IRC_SERV));
		}

		int res = poll(m_clientFds.data(), m_clientFds.size(), m_timeout);
		if (res != 0) {
			for (size_t i = 0; i < m_clientFds.size(); ++i) {
				if (m_clientFds[i].revents & POLLIN) {
					if ((m_users[i]->readMes() == -1) || (handleMes(*(m_users[i])) == -1))
						m_users[i]->setFlag(BREAKCONNECTION);
				}
				m_clientFds[i].revents = 0;
			}
		}

		std::string name = IRC_SERV;
		for (size_t i = 0; i < m_users.size(); i++) {
			if (this->m_users[i]->getFlags() & REGISTERED) {
				if (time(0) - this->m_users[i]->getTimeLastMes() > static_cast<time_t>(120)) {
					this->m_users[i]->sendMessage(":" + name + " PING :" + IRC_SERV + "\n");
					this->m_users[i]->updateTimePing();
					this->m_users[i]->updateTimeLastMes();
					this->m_users[i]->setFlag(PINGING);
				}
				if ((m_users[i]->getFlags() & PINGING) && time(0) - m_users[i]->getTimePing() > static_cast<time_t>(60))
					m_users[i]->setFlag(BREAKCONNECTION);
			}
		}

		for (size_t i = 0; i < m_users.size(); ++i) {
			if (m_users[i]->getFlags() & BREAKCONNECTION) {
				m_history.addUser(*(m_users[i]));
				notification(*(m_users[i]), ": " + m_users[i]->getPrefix() + "QUIT :" + m_users[i]->getExitMsg() + "\n");
				close(m_users[i]->getSocket());
				for (std::map<std::string, Chat *>::iterator it = chats.begin(); it != chats.end(); ++it)
					if (it->second->verifyUsers(m_users[i]->getNick()))
						it->second->disconnect(*(m_users[i]));
				delete m_users[i];
				m_users.erase(m_users.begin() + i);
				m_clientFds.erase(m_clientFds.begin() + i);
				--i;
			}
		}

		for (std::map<std::string, Chat *>::const_iterator it = chats.begin(); it != chats.end(); ) {
			if (it->second->isEmpty()) {
				delete it->second;
				chats.erase((*it).first);
				it = chats.begin();
			} else
				++it;
		}
	}
}
