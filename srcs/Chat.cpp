#include "../includes/Chat.hpp"

Chat::Chat(const std::string &chat_name, const Client &admin, const std::string &chat_password):
		name(chat_name), password(chat_password), client_limit(0), flags(NOMSGOUT)
{
	members.push_back(&admin);
	admins.push_back(&admin);
	display_chat_info(admin);
}

Chat::~Chat()
{}

void Chat::display_chat_info(const Client &member)
{
	send_message(" joined!", member);
	display_topic(member);
	display_members(member);
	send_reply(SERVER_NAME, member, 366, name);
}

void Chat::display_topic(const Client &member)
{
	if (topic.size() > 0)
		send_reply(SERVER_NAME, member, 332, name, topic);
	else
		send_reply(SERVER_NAME, member, 331, name);
}

void Chat::display_members(const Client& member)
{
	std::string members_nicks;
	const Client* user;
	const_iterator it_begin = members.begin();
	const_iterator it_end = members.end();
	while(it_begin != it_end)
	{
		user = *it_begin;
		if (is_admin(*user))
			members_nicks += "@";
		else if (is_speaker(*user))
			members_nicks += "+";
		members_nicks += user->get_nick();
		++it_begin;
		if (it_begin != it_end)
			members_nicks += " ";
	}
	send_reply(SERVER_NAME, member, 353, ": " + name, members_nicks);
}

bool Chat::is_admin(const Client &member) const
{
	for (size_t i = 0; i < admins.size(); ++i)
		if (admins[i]->get_prefix() == member.get_prefix())
			return true;
	return false;
}

bool Chat::is_speaker(const Client &member) const

{
	for (size_t i = 0; i < speakers.size(); ++i)
		if (speakers[i]->get_prefix() == member.get_prefix())
			return true;
	return false;
}

bool Chat::is_empty() const
{
	if (members.size() == 0)
		return true;
	return false;
}

void Chat::send_message(const std::string &message, const Client &sender) const
{
	std::string text = sender.get_nick() + ": " + message;
	for (const_iterator it = members.begin(); it != members.end(); ++it)
		if (*it != &sender)
			(*it)->send_message(text);
}

const std::string& Chat::get_name() const
{
	return name;
}

const unsigned char& Chat::get_flags() const
{
	return flags;
}

bool Chat::is_invited(const Client &member) const
{
	for (size_t i = 0; i < invited_members.size(); ++i)
		if (invited_members[i]->get_username() == member.get_prefix())
			return true;
	return false;
}

bool Chat::is_exist_member(const std::string &nickname) const
{
	for (const_iterator it = members.begin(); it != members.end(); ++it)
		if ((*it)->get_nick() == nickname)
			return true;
	return false;
}

bool Chat::is_member_banned(const std::string &mask, const std::string &prefix)
{
	return is_equal_to_regex(mask, prefix);
}

void	Chat::connect(const Client &member, const std::string &pass)
{
	if ((flags & PRIVATE) && pass != password)
		send_error(member, 475, name);
	else if (client_limit != 0 && members.size() >= client_limit)
		send_error(member, 471, name);
	else if ((flags & INVITEONLY) && !is_invited(member))
		send_error(member, 473, name);
	else
	{
		for (size_t i = 0; i < ban_masks.size(); i++)
		{
			if (is_member_banned(ban_masks[i], member.get_prefix()))
			{
				send_error(member, 474, name);
				return ;
			}
		}
		for (const_iterator it = members.begin(); it != members.end(); ++it)
			if ((*it)->get_prefix() == member.get_prefix())
				return ;
		members.push_back(&member);
		remove_invited(member);
		display_chat_info(member);
	}
}

void Chat::remove_invited(const Client &member)
{
	if (is_invited(member))
	{
		size_t i;
		for (i = 0; i < invited_members.size(); ++i)
			if (invited_members[i] == &member)
				break;
		invited_members.erase(invited_members.begin() + i);
	}
}

void Chat::invite(const Client &member, const Client &inviter)
{
	if (flags & INVITEONLY && !is_admin(member))
		send_error(member, 482, name);
	else
	{
		invited_members.push_back(&inviter);
		inviter.send_message(":" + member.get_prefix() + " INVITE " + name + "\n");
		send_reply(SERVER_NAME, member, 341, name, inviter.get_nick());
		if (inviter.get_flags() & AWAY)
			send_reply(SERVER_NAME, member, 301, inviter.get_nick(), inviter.get_exit_msg());
	}
}

void	Chat::remove_admin(const Client &member)
{
	if (is_admin(member))
	{
		size_t	i;
		for (i = 0; i < admins.size(); i++)
			if (admins[i] == &member)
				break;
		admins.erase(admins.begin() + i);
		if (admins.size() == 0 && members.size() > 0)
		{
			admins.push_back(members[0]);
			send_message("MODE " + this->name + " +o "  + members[0]->get_nick() + "\n", member);
		}
	}
}

void Chat::remove_speaker(const Client &member)
{
	if (is_speaker(member))
	{
		size_t	i;
		for (i = 0; i < speakers.size(); i++)
			if (speakers[i] == &member)
				break;
		speakers.erase(speakers.begin() + i);
	}
}

void Chat::disconnect(const Client& user)
{
	const_iterator it = members.begin();
	for (; it != members.end(); ++it)
		if (*it == &user)
			break ;
	members.erase(it);
	remove_admin(user);
	remove_speaker(user);
}

void Chat::member_chat_info(const Client &member)
{
	std::string	chat_name = "";
	std::string	info = "";
	if ((flags & SECRET) && !is_exist_member(member.get_nick()))
		return ;
	if ((flags & PRIVATE) && !is_exist_member(member.get_nick()))
		chat_name = "*";
	else
	{
		chat_name = name;
		info = "[+" + str_flags() + "] " + topic;
	}
	std::stringstream	ss;
	ss << members.size();
	send_reply(SERVER_NAME, member, 322, chat_name, ss.str(), info);
}

std::string	Chat::str_flags() const
{
	std::string	res;
	if (flags & INVITEONLY)
		res += "i";
	if (flags & NOMSGOUT)
		res += "n";
	if (flags & PRIVATE)
		res += "p";
	if (flags & SECRET)
		res += "s";
	if (flags & TOPICSET)
		res += "t";
	return res;
}
