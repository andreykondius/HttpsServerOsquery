#include "forwardmanager.h"
#include "tlsserver.h"

#include "json11.hpp"
#include <iostream>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>


namespace lib {

ForwardManager::ForwardManager()
{
	std::string host("localhost");  //@TODO : load from config of ClickHouse instead
	m_options.SetHost(host);
//	options.SetPort(8123);

	m_metrics.timestamp = std::make_shared<clickhouse::ColumnString>();
	m_metrics.value		= std::make_shared<clickhouse::ColumnUInt64>();
	m_metrics.id_desc	= std::make_shared<clickhouse::ColumnUInt64>();
	m_metrics.id_host	= std::make_shared<clickhouse::ColumnUInt64>();

	m_clickHouseClient = std::make_shared<clickhouse::Client>(m_options);

	m_agents = std::make_shared<AgentsManager>();

	auto thr = std::async(std::launch::async, [this](){
		m_serv = std::make_shared<TlsServer>(m_agents, 8080);
		std::function<void(const char *data)> func = boost::bind(&ForwardManager::onData, this, _1);
		m_serv->setConnect(func);
		m_serv->start();
	});

	std::cout << "Server was run." << std::endl;
}

ForwardManager::~ForwardManager()
{
	std::cout << "Server was stop." << std::endl;
}

void ForwardManager::forwardMetrics()
{
	clickhouse::Block block;
	block.AppendColumn("timestamp",		m_metrics.timestamp);
	block.AppendColumn("value",			m_metrics.value);
	block.AppendColumn("id_host",		m_metrics.id_host);
	block.AppendColumn("id_desc",		m_metrics.id_desc);
	m_clickHouseClient->Insert("metrics.base", block);
	m_metrics.Clear();

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void ForwardManager::onData(const char *data)
{
	std::string request (data);
	if (!getMetricsRequest(request))
		return;

	forwardMetrics();
}
uint64_t ForwardManager::idFieldNameValue(const std::string &table, const std::string &value)
{
	uint64_t id = 0;
	std::string query = "SELECT id FROM "+table+" where name ='"+value+"'";
	m_clickHouseClient->Select(query, [&] (const clickhouse::Block& block)->uint64_t {
		if (block.GetRowCount()) {
			id = block[0]->As<clickhouse::ColumnUInt64>()->At(0);
			return id;
		}
		return id;
	});
	return id;
}

uint64_t ForwardManager::forwDict(const std::string &table, const std::string &value)
{
	auto id = idFieldNameValue(table, value);
	if (!id) {
		m_id_max = std::max(id, m_id_max)+1;
		id = m_id_max;
		std::shared_ptr<clickhouse::ColumnUInt64> id_table = std::make_shared<clickhouse::ColumnUInt64>();
		std::shared_ptr<clickhouse::ColumnString> name_table = std::make_shared<clickhouse::ColumnString>();
		id_table->Append(id);
		name_table->Append(value);

		clickhouse::Block block;
		block.AppendColumn("name",		name_table);
		block.AppendColumn("id",		id_table);
		m_clickHouseClient->Insert(table, block);
	}
	return id;
}

bool ForwardManager::getMetricsRequest(const std::string &request)
{
	std::string err;
	json11::Json jsonRoot = json11::Json::parse((const char*)request.c_str(), err);
	if (!err.empty() || jsonRoot["log_type"].is_null() || jsonRoot["log_type"] != "result")
		return false;

	json11::Json j_data = jsonRoot["data"];

	for (auto it : j_data.array_items()) {
		std::string calendarTime = it["calendarTime"].string_value();
		std::string hostIdentifier = it["hostIdentifier"].string_value();
		json11::Json j_items = it["columns"];
		std::cout << calendarTime << ": ";
		int i = 0;
		for (auto it_m : j_items.object_items()) {
			if (i)
				std::cout << ", ";
			++i;
			std::string metricName = it_m.first;
			int metricValue = 0;
			try {
				metricValue = std::stod(j_items[metricName].string_value());
			} catch(...) {}

			auto id_desc = forwDict("metrics.desc", metricName);
			auto id_host = forwDict("metrics.hosts", hostIdentifier);

			m_metrics.timestamp->Append(calendarTime);
			m_metrics.value->Append( metricValue );
			m_metrics.id_desc->Append( id_desc );
			m_metrics.id_host->Append( id_host );
			std::cout << metricName << "=" << metricValue;
		}
		std::cout << std::endl;
	}
	return true;
}

}	// namespace lib
