#pragma once
#include "Message.hpp"

class Information {
private:
	Information();
	Information(const Information &copy);
	Information &operator=(const Information &copy);

	std::string nickname;
	std::string hostname;
	std::string username;
	time_t registration_time;

public:
	Information(const Client &user);
	~Information();
};
