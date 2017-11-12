//
// Created by 49844 on 12/11/2017.
//

#include "Server.hpp"

namespace protocol {
    void InputConnection::read() {
        auto read_socket = [&, this](std::error_code ec, std::size_t length) {
            std::cout << "Reading something of " << length << std::endl;
            if (!ec) {
                std::cout << m_data.data() << std::endl;
                read();
            } else {
                std::cerr << "Error " << ec.message() << std::endl;
            }
        };
        asio::async_read(*m_socket, asio::buffer(m_data, 25), read_socket);
    }

    void InputConnection::on_accept() {
        std::cout << "Start reading datas" << std::endl;
        read();
    }
}

namespace protocol {
    void Server::accept() {
        auto accept_complete = [&, this] (std::error_code ec) {
            if (!ec) {
                std::cout << "New connection occur ..." << std::endl;
                auto connection = std::make_unique<InputConnection>(*this, std::move(m_socket));
                connection->on_accept();
                m_connections.push_back(std::move(connection));
                m_socket = std::make_unique<tcp::socket>(m_service);
                accept();
            }
        };
        m_acceptor.async_accept(*m_socket, accept_complete);
    }

    void Server::run() {
        accept();
        m_service.run();
    }

    void Server::stop() {
        m_service.stop();
    }
}