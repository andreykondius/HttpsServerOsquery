#pragma once
#include <string>

namespace lib {

enum TypePath
{
	HTTP_ENROLL,
	HTTP_CONFIG,
	HTTP_LOGGER,
	HTTP_OTHER
};

class Serializer
{
public:
	Serializer(const char *data, const unsigned int len);
	int getPort() const;
	TypePath getType() const;
	std::string getData() const;
	std::string getBody() const;
	std::string getPath() const;
	std::string getCommand() const;
	std::string getHost() const;
	std::string getHostName() const;
	std::string getProtocolVersion() const;

private:
	void dataParsing();
	template<class T>
	T getValue(const std::string tag_start, const std::string tag_end, const int pos=0);
	int getValue(const std::string tag_start, const std::string tag_end, const int pos=0);

private:
	unsigned int m_port;
	std::string m_nodeSecret = "this_is_a_node_secret";	//@TODO : load from config instead

	std::string m_config = "{"
						   "	\"schedule\": {"
						   "		\"cpu_time\": {"
						   "			\"query\": \"select (SELECT sum(user_time)+sum(system_time) FROM processes) as processes_usage, "
															"(SELECT sum(user)+sum(nice)+sum(system)+sum(idle)+sum(iowait)+sum(irq)+"
																	"sum(softirq)+sum(steal)+sum(guest)+sum(guest_nice) FROM cpu_time) as cpu_usage, "
															"(SELECT memory_free FROM  memory_info) as memory_free, "
															"(SELECT inactive FROM memory_info) as swap_free\", "
						   "			\"interval\": 15"
						   "		}"
						   "	}, "
						   "	\"node_invalid\": false, "
						   "	\"node\": true"
						   "}\n";	//@TODO : load from config instead
	TypePath m_type;
	std::string m_path;
	std::string m_command;
	std::string m_data;
	std::string m_host;
	std::string m_hostName;
	std::string m_protocolVersion;
	std::string m_body;
};

}	// namespace lib
