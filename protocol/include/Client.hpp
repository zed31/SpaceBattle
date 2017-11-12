//
// Created by 49844 on 12/11/2017.
//

#ifndef SPACE_BATTLE_CLIENT_HPP
#define SPACE_BATTLE_CLIENT_HPP

#include <thread>
#include <memory>
#include <functional>
#include <string>
#include "asio.hpp"

using asio::ip::tcp;

namespace protocol {

class OutputConnection{
public:
    OutputConnection(tcp::socket &&m_socket);
    void read();
    void close();
private:
    tcp::socket m_socket;
    std::array<char, 25> m_data;
};

class Client {
    using on_connect_function = std::function<void()>;
public:
    Client(const std::string &host, const std::string &port);
    void connect(const on_connect_function &onConnect);
    void run(const on_connect_function &onConnect);
private:
    asio::io_service m_service;
    asio::io_service::work m_service_worker;
    tcp::resolver m_resolver;
    tcp::socket m_socket;
    std::thread m_service_startup;
    std::unique_ptr<OutputConnection> m_output;
    std::string m_host;
    std::string m_port;
};

} // namespace protocol

#endif //SPACE_BATTLE_CLIENT_HPP
