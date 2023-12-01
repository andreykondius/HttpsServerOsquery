#pragma once

#include "agentsmanager.h"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/signals2.hpp>
#include <memory>
#include <functional>

namespace lib {
using tcp = boost::asio::ip::tcp;

class TlsSession : public std::enable_shared_from_this<TlsSession>
{
public:
	TlsSession();
	TlsSession(tcp::socket sock,
			   boost::asio::ssl::context& context,
			   std::shared_ptr<lib::AgentsManager> clients);
	void start();
	boost::signals2::connection setConnect(std::function<void(const char *data)> &func);

private:
	void do_handshake();
	void do_read();
	void do_write(std::size_t length);

private:
	char m_data[4096];		//@TODO: change to unlimit buffer
	boost::asio::ssl::stream<tcp::socket> m_socket;
	std::shared_ptr<lib::AgentsManager> m_agents;
	boost::signals2::signal<void(const char *data)> signalData;

};

}	// namespace lib
