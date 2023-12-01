#pragma once
#include "agentsmanager.h"
#include "json11.hpp"

#include <functional>
#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/signals2.hpp>
#include <string>

using tcp = boost::asio::ip::tcp;

namespace lib {

class TlsSession;

class TlsServer
{
public:
	TlsServer(std::shared_ptr<AgentsManager> agents, const unsigned short port);
	void start();
	boost::signals2::connection setConnect(std::function<void (const char *)> &func);

private:
	void do_accept();
	std::string get_password() const;

private:
	boost::asio::io_context m_ioContext;
	boost::asio::ssl::context m_context;
	std::shared_ptr<AgentsManager> m_agents;
	tcp::acceptor m_acceptor;
	std::shared_ptr<TlsSession> m_session;
	boost::signals2::signal<void(const char *data)> signalData;

};

}
