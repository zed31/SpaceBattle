//
// Created by 49844 on 12/11/2017.
//

#include <iostream>
#include "Client.hpp"

namespace protocol {
    OutputConnection::OutputConnection(tcp::socket &&m_socket)
    : m_socket{ std::move(m_socket) } {}

    void OutputConnection::read() {
        auto read_socket = [&, this] (std::error_code ec, std::size_t length) {
            std::cout << "OutputConnection::read " << length << "bytes" << std::endl;
            if (!ec) {
                std::cerr << "OutputConnection::read : Read success" << std::endl;
            } else {
                std::cerr << "OutputConnection::Error : " << ec.message() << std::endl;
            }
        };
        asio::async_read(m_socket, asio::buffer(m_data, 25), read_socket);
    }

    void OutputConnection::close() {
        m_socket.close();
    }
}

namespace protocol {

    Client::Client(const std::string &host, const std::string &port)
        : m_service_worker{m_service}, m_resolver{ m_service },
          m_socket{ m_service }, m_service_startup{ [this]() { m_service.run(); } }, m_host{ host }, m_port{ port } {
    }

    void Client::connect(const on_connect_function &onConnect) {
        auto connect_socket = [&, this] (std::error_code ec, tcp::resolver::iterator) {
            if (!ec) {
                std::cout << "Client::connect Connection complete" << std::endl;
                //m_output = std::make_unique<OutputConnection>(std::move(m_socket));
                onConnect();
            } else {
                std::cerr << "Client::connect Error on connection : " << ec.message() << std::endl;
            }
        };
        auto endpoint_iterator = m_resolver.resolve({m_host, m_port});
        asio::async_connect(m_socket, endpoint_iterator, connect_socket);
    }

    void Client::run(const on_connect_function &onConnect) {
        connect(onConnect);
    }

}
