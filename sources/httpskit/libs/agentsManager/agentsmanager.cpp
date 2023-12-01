#include "agentsmanager.h"

namespace lib {

void AgentsManager::push(const std::string &host, const AgentData &data)
{
	m_agents[host] = data;
}

void AgentsManager::pop(const std::string &host)
{
	auto it = m_agents.find(host);
	if (it != m_agents.end())
		m_agents.erase(it);
}

}	// namespace lib
 
