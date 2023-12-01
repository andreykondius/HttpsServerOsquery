#pragma once

#include <clickhouse/client.h>
#include <memory>

namespace lib {

class AgentsManager;
class TlsServer;

struct MetricsBase {
	std::shared_ptr<clickhouse::ColumnString> timestamp;
	std::shared_ptr<clickhouse::ColumnUInt64> id_host;
	std::shared_ptr<clickhouse::ColumnUInt64> id_desc;
	std::shared_ptr<clickhouse::ColumnUInt64> value;
	void Clear() {
		timestamp->Clear();
		value->Clear();
		id_host->Clear();
		id_desc->Clear();
	}
};

class ForwardManager
{
public:
	ForwardManager();
	~ForwardManager();

	void forwardMetrics();
	void onData(const char *data);

private:
	bool getMetricsRequest(const std::string &request);
	uint64_t idFieldNameValue(const std::string &table, const std::string &value);
	uint64_t forwDict(const std::string &table, const std::string &value);

private:
	int m_seek = 0;
	clickhouse::ClientOptions m_options;
	std::shared_ptr<clickhouse::Client> m_clickHouseClient;
	std::shared_ptr<TlsServer> m_serv;
	std::shared_ptr<AgentsManager> m_agents;

	uint64_t m_id_max = 0;
	MetricsBase m_metrics;

};

}	// namespace lib
