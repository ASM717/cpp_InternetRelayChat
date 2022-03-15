#include "../includes/History.hpp"

History::History() {}

History::~History()
{
	while (history_list.size() > 0)
	{
		delete history_list[0];
		history_list.erase(history_list.begin());
	}
}
void History::add_user(const Client &member)
{
	this->history_list.push_back(new UserInfo(member));
}