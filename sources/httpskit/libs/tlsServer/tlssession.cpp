#include "tlssession.h"
#include "serializer.h"

#include <ctime>

namespace lib {

TlsSession::TlsSession(tcp::socket sock,
					   boost::asio::ssl::context& context,
					   std::shared_ptr<AgentsManager> agents)
	: m_socket(std::move(sock), context),
	m_agents(agents)
{
}

void TlsSession::start()
{
	do_handshake();
}

void TlsSession::do_handshake()
{
	auto self(shared_from_this());
	m_socket.async_handshake(boost::asio::ssl::stream_base::server,
							[this, self](const boost::system::error_code& error)
							{
								if (!error)
								{
									do_read();
								}
							 });
}
void TlsSession::do_write(std::size_t length)
{
	auto self(shared_from_this());
	boost::asio::async_write(m_socket, boost::asio::buffer(m_data, length),
							 [this, self](const boost::system::error_code& ec,
										  std::size_t)
							 {
								 if (!ec)
								 {
//									 do_read();
								 }
	});
}

boost::signals2::connection TlsSession::setConnect(std::function<void(const char *data)> &func)
{
	return signalData.connect(func);
}

void TlsSession::do_read()
{
	auto self(shared_from_this());
	m_socket.async_read_some(boost::asio::buffer(m_data),
							[this, self](const boost::system::error_code& ec, std::size_t length)
							{
								if (!ec)
								{
									Serializer serializer(m_data, length);
									if (serializer.getType() == HTTP_ENROLL) {
										m_agents->push(serializer.getHost(), {serializer.getPort(),
																				serializer.getHostName(),
																				serializer.getProtocolVersion()});
									}

									std::string response_buffer ("HTTP/1.1 200 OK\r\nServer: BaseHTTP/0.6 Python/3.10.12\r\nDate: ");
									time_t curr_time;
									curr_time = time(NULL);
									tm *tm_gmt = gmtime(&curr_time);
									std::string date = asctime(tm_gmt);
									response_buffer += date.substr(0, date.length()-1);
									response_buffer += "\r\nContent-Type: application/json\r\nContent-Length: ";
									std::string body = serializer.getBody();
									if (serializer.getType() == HTTP_LOGGER) {
										signalData(serializer.getBody().c_str());
										body = "{\"foo\": \"bar\"}\n";
									}
									response_buffer += std::to_string(body.length());
									response_buffer += "\r\n\r\n ";
									response_buffer += body;

									if (serializer.getType() != HTTP_OTHER) {
										int l = response_buffer.length();
										boost::asio::async_write(m_socket, boost::asio::buffer(response_buffer, l),
																 [this, self](const boost::system::error_code& ec,std::size_t){});
									}
								}
							});
}

}	// namespace lib
