#pragma once
#include <unordered_map>
#include <string>

namespace lib {

struct AgentData
{
	unsigned int port;
	std::string hostName;
	std::string protocol_version;
};

class AgentsManager
{
public:
	void push(const std::string &host, const AgentData &data);
	void pop(const std::string &host);

private:
	std::unordered_map<std::string, AgentData> m_agents;	// [host, data]
};

}	// namespace lib
  
