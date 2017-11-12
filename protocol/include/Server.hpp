//
// Created by 49844 on 12/11/2017.
//

#ifndef SPACE_BATTLE_CONNECTIONLISTENER_HPP
#define SPACE_BATTLE_CONNECTIONLISTENER_HPP

#include <iostream>
#include <memory>
#include <array>
#include "asio.hpp"

using asio::ip::tcp;

namespace protocol {

class InputConnection {
    friend class Server;
public:
    InputConnection(Server &server, std::unique_ptr<tcp::socket> &&socket)
        : m_server{ server }, m_socket{ std::move(socket) } {};
private:
    void on_accept();
    void read();
private:
    Server &m_server;
    std::unique_ptr<tcp::socket> m_socket;
    std::array<char, 25> m_data;
};

class Server {
public:
    Server(unsigned short port)
        : m_endpoint{ tcp::v4(), port }, m_acceptor{ m_service, m_endpoint }, m_socket{ std::make_unique<tcp::socket>(m_service) } {};

    void run();
    void stop();
private:
    void accept();
private:
    asio::io_service m_service;
    tcp::endpoint m_endpoint;
    tcp::acceptor m_acceptor;
    std::unique_ptr<tcp::socket> m_socket;
    std::vector<std::unique_ptr<InputConnection>> m_connections{};
};

} // namespace protocol

#endif //SPACE_BATTLE_CONNECTIONLISTENER_HPP
