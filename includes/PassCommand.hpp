#pragma once
#include "Message.hpp"



class PassCommand {
private:

public:
	PassCommand(const Message &msg, Client &member);
	~PassCommand();
//	int pass_cmd(const Message &msg, Client &member)
//	{
//		if (msg.get_params().size() == 0)
//			send_error(member, ERR_NEEDMOREPARAMS, msg.get_command());
//		else if (member.get_flags() & REGISTERED)
//			send_error(member, ERR_ALREADYREGISTRED);
//		else
//			member.set_password(msg.get_params()[0]);
//		return 0;
//	}
};