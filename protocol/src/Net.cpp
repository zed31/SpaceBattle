//
// Created by 49844 on 06/11/2017.
//

#include <memory>
#include "Net.hpp"

namespace protocol {

//Net implementation

Net::Net(asio::io_service &io_service)
    : m_io_service{ &io_service } {}

//NetServer implementation

NetServer::NetServer(asio::io_service &io_service, const tcp::endpoint &endpoint, unsigned limit)
    : Net{ io_service }, m_socket{ std::make_unique<tcp::socket>(io_service) }, m_acceptor{ io_service, endpoint }, m_limit{ limit } {}

//NetClient implementation
NetClient::NetClient(asio::io_service &io_service, tcp::resolver::iterator endpoint_iterator)
     : Net{ io_service } , m_endpoint_iterator{ endpoint_iterator } , m_socket{ std::make_unique<tcp::socket>(io_service) } {}

void NetClient::close() {
    Net::m_io_service->post([this] () { m_socket->close(); });
}

}
