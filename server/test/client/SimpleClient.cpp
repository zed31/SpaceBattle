//
// Created by 49844 on 04/11/2017.
//

#include <iostream>
#include "simple_client.hh"

namespace test {

SimpleClient::SimpleClient(asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator)
    : m_io_service{ io_service }, m_socket{ io_service } {
    asio::async_connect(m_socket, std::move(endpoint_iterator), [this](std::error_code ec, tcp::resolver::iterator) {
        if (!ec) {
            std::cout << "connection successfully complete" << std::endl;
        } else {
            std::cout << "connection failed because : " << ec.message() << std::endl;
        }
    });
}

void SimpleClient::stop() {
    m_io_service.post([this] () { m_socket.close(); });
}

}
