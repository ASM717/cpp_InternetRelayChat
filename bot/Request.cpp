#include "Request.hpp"

Request::Request(const std::string &Request) {
	m_request = std::string(Request.begin(), Request.end() - 1);
	std::string::iterator spaceX;

	for(std::string::iterator it = m_request.begin(); it != m_request.end();)
	{
		while(it != m_request.end() && *it == ' ')
			++it;
		spaceX = std::find(it, m_request.end(), ' ');
		if (it != m_request.end())
		{
			m_paramsQueue.push(std::string(it, spaceX));
			it = spaceX;
		}
	}

	if (m_paramsQueue.size() > 0 && m_paramsQueue.front()[0] == ':')
	{
		m_prefix = std::string(m_paramsQueue.front().begin() + 1, m_paramsQueue.front().end());
		m_paramsQueue.pop();
	}

	if (m_paramsQueue.size() > 0)
	{
		m_command = m_paramsQueue.front();
		m_paramsQueue.pop();
	}

	while (m_paramsQueue.size() > 0)
	{
		if (m_paramsQueue.front()[0] == ':')
		{
			std::string param(m_paramsQueue.front().begin() + 1, m_paramsQueue.front().end());
			m_paramsQueue.pop();
			while (m_paramsQueue.size() > 0)
			{
				param.append(" ");
				param.append(m_paramsQueue.front());
				m_paramsQueue.pop();
			}
			m_params.push_back(param);
		}
		else
		{
			m_params.push_back(m_paramsQueue.front());
			m_paramsQueue.pop();
		}
	}
}

Request::~Request()
{

}

const std::string& Request::getPrefix() const
{
	return (m_prefix);
}

const std::string& Request::getCommand() const
{
	return (m_command);
}

const std::vector<std::string>& Request::getParams() const
{
	return (m_params);
}
