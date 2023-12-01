#include "serializer.h"
#include <string.h>
#include "json11.hpp"

namespace lib {

Serializer::Serializer(const char *data, const unsigned int len)
{	
	for(auto i=0; i < len; ++i) {
		if ((i+5) < len
			&& data[i] == '\\'
			&& data[i+1] == 'u'
			&& data[i+2] == '0'
			&& data[i+3] == '0'
			&& data[i+4] == '0'
			&& data[i+5] == '0'
			) {
			i += 5;
			continue;
		}
		m_data += data[i];
	}

	dataParsing();
}

template<class T>
T Serializer::getValue(const std::string tag_start, const std::string tag_end, const int pos)
{
	int start = m_data.find(tag_start, pos) + tag_start.length();
	int end = m_data.find(tag_end, start);
	T res = m_data.substr(start, end-start);
	return res;
}

int Serializer::getValue(const std::string tag_start, const std::string tag_end, const int pos)
{
	int res = 0;
	try {
		res = atoi(getValue<std::string>(tag_start, tag_end, pos).c_str());
	} catch(...) {}
	return res;
}

void Serializer::dataParsing()
{
	m_command = getValue<std::string>("", " ");
	int pos = m_data.find(" ")+1;
	m_path = getValue<std::string>("", " ", pos);
	m_type = HTTP_OTHER;
	if (m_path == "/enroll") {
		m_type = HTTP_ENROLL;
		pos = m_data.find(" ", pos+m_path.length())+1;
		m_protocolVersion = getValue<std::string>("", "\r", pos);
		m_host = getValue<std::string>("Host: ", ":");
		pos = m_data.find("Host: ")+5;
		try {
			m_port = getValue(":", " ", pos);
		} catch (...) {};

		m_data.erase(0, m_data.find("{"));

		std::string err;
		json11::Json request = json11::Json::parse(m_data, err);
		json11::Json host_details = request["host_details"];
		json11::Json sysinfo = host_details["system_info"];
		m_hostName = sysinfo["hostname"].string_value();

		m_body = "{\"node_key\": \"" + m_nodeSecret + "\"}\n";
	} else if (m_path == "/config") {
		m_type = HTTP_CONFIG;
		m_body = m_config;
	}else if (m_path == "/logger") {
		m_data.erase(0, m_data.find("{"));
		m_type = HTTP_LOGGER;
		m_body = m_data+"\n";
	}
}

int Serializer::getPort() const
{
	return m_port;
}

TypePath Serializer::getType() const
{
	return m_type;
}

std::string Serializer::getData() const
{
	return m_data;
}

std::string Serializer::getBody() const
{
	return m_body;
}

std::string Serializer::getPath() const
{
	return m_path;
}

std::string Serializer::getCommand() const
{
	return m_command;
}

std::string Serializer::getHost() const
{
	return m_host;
}

std::string Serializer::getHostName() const
{
	return m_hostName;
}

std::string Serializer::getProtocolVersion() const
{
	return m_protocolVersion;
}

}	// namespace lib
