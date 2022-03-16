#include "../includes/Information.hpp"

Information::Information(const Client &member)
{
	nickname = member.getNick();
	username = member.getUsername();
	hostname = member.getIpHost();
	registration_time = time(0);
}

Information::~Information() { }