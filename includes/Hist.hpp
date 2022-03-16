#pragma once

#include "Message.hpp"

class Information;

class Hist {
private:
	Hist(const Hist &copy);
	Hist &operator=(const Hist &copy);

	std::vector<Information*> historyList;

public:
	Hist();
	~Hist();

	void addUser(const Client &client);
};
