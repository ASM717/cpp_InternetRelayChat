#include "../includes/Hist.hpp"

Hist::Hist() {}

Hist::~Hist()
{
	while (historyList.size() > 0)
	{
		delete historyList[0];
		historyList.erase(historyList.begin());
	}
}
void Hist::addUser(const Client &client)
{
	this->historyList.push_back(new Information(client));
}