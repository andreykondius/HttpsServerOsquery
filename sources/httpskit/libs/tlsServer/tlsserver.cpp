#include "tlsserver.h"
#include "tlssession.h"

namespace lib {

TlsServer::TlsServer(std::shared_ptr<AgentsManager> agents,
					 const unsigned short port)
	: m_acceptor(m_ioContext, tcp::endpoint(tcp::v4(), port)),
	m_context(boost::asio::ssl::context::sslv23),
	m_agents(agents)
{
	m_context.set_options(
		boost::asio::ssl::context::default_workarounds
		| boost::asio::ssl::context::no_sslv2
		| boost::asio::ssl::context::single_dh_use);
	m_context.set_password_callback(std::bind(&TlsServer::get_password, this));
	m_context.use_certificate_chain_file("certificates/server.pem");
	m_context.load_verify_file("certificates/server_ca.pem");
	m_context.use_private_key_file("certificates/server.key", boost::asio::ssl::context::pem);
}

void TlsServer::start()
{
	do_accept();
	m_ioContext.run();
}

boost::signals2::connection TlsServer::setConnect(std::function<void(const char *data)> &func)
{
	return signalData.connect(func);
}

std::string TlsServer::get_password() const
{
	return "test";
}

void TlsServer::do_accept()
{
	m_acceptor.async_accept(
		[this](const boost::system::error_code& error, tcp::socket socket)
		{
			if (!error)
			{
				std::shared_ptr<TlsSession> session = std::make_shared<TlsSession>(std::move(socket), m_context, m_agents);

				std::function<void(const char *data)> func = [this](const char *data) {
					signalData(data);
				};
				session->setConnect(func);

				session->start();
			}

			do_accept();
		});
}

}  // namespace lib
