#include "PassCommand.hpp"

PassCommand::PassCommand(const Message &msg, Client &member) {
	if (msg.get_params().size() == 0)
		send_error(member, 461, msg.get_command());
	else if (member.get_flags() & REGISTERED)
		send_error(member, 462);
	else
		member.set_password(msg.get_params()[0]);
}

PassCommand::~PassCommand() {

}