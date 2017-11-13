//
// Created by 49844 on 12/11/2017.
//

#ifndef SPACE_BATTLE_CONNECTIONLISTENER_HPP
#define SPACE_BATTLE_CONNECTIONLISTENER_HPP

#include <iostream>
#include <memory>
#include <array>
#include <functional>
#include "asio.hpp"
#include "serialization/response.hpp"
#include "serialization/request.hpp"

using asio::ip::tcp;

namespace protocol {

class InputConnection {
    friend class server;
public:
    InputConnection(server &server, asio::io_service &service)
        : m_server{ server }, m_socket{ service } {};
    void write();
private:
    void on_accept();
    void read();
    void read_body(const serialize::HeaderRequest &request);
    void close();
private:
    std::function<void(const serialize::Request &, InputConnection &)> m_on_read;
    std::function<void(InputConnection &)> m_on_write_succeed;
    std::function<void(std::error_code, InputConnection &)> m_on_write_failed;
    std::function<void(std::error_code, InputConnection &)> m_on_read_failed;
    std::function<void()> m_on_close;
    server &m_server;
    tcp::socket m_socket;
    std::array<char, 25> m_data;
    char *m_body;
    serialize::HeaderRequest m_current_header_request;
};

class server {
public:
    server(unsigned short port)
        : m_endpoint{ tcp::v4(), port }, m_acceptor{ m_service, m_endpoint } {};

    void run();
    void on_write_succeed(const std::function<void(InputConnection &)> &onWriteSucceed);
    void on_write_failed(const std::function<void(std::error_code, InputConnection &)> &onWriteFaied);
    void on_read_succeed(const std::function<void(const serialize::Request &, InputConnection &)> &onReadSucceed);
    void on_read_failed(const std::function<void(std::error_code, InputConnection &)> &onReadFailed);
    void on_close(const std::function<void()> &onClose);
    void stop();
private:
    void accept();
private:
    std::function<void(const serialize::Request &, InputConnection &)> m_on_read;
    std::function<void(InputConnection &)> m_on_write_succeed;
    std::function<void(std::error_code, InputConnection &)> m_on_write_failed;
    std::function<void(std::error_code, InputConnection &)> m_on_read_failed;
    std::function<void()> m_on_close;
    asio::io_service m_service;
    tcp::endpoint m_endpoint;
    tcp::acceptor m_acceptor;
    std::unique_ptr<InputConnection> m_waiting_connection;
    std::vector<std::unique_ptr<InputConnection>> m_connections{};
};

} // namespace protocol

#endif //SPACE_BATTLE_CONNECTIONLISTENER_HPP
