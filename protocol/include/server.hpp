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

class InputConnection;
class server;
using on_write_success_t = std::function<void(const serialize::Response &, InputConnection &)>;
using on_read_t = std::function<void(const serialize::Request &, InputConnection &)>;
using on_write_failed_t = std::function<void(std::error_code, InputConnection &)>;
using on_read_failed_t = std::function<void(std::error_code, InputConnection &)>;
using on_close_t = std::function<void()>;
using on_accept_t = std::function<void()>;

class InputConnection {
    friend class server;
public:
    InputConnection(server &server, asio::io_service &service)
        : m_server{ server }, m_socket{ service } {};
    ~InputConnection();
    void write(const serialize::Response &response);
private:
    void on_accept();
    void read();
    void read_body(const serialize::HeaderRequest &request);
    void close();
private:
    on_read_t m_on_read;
    on_write_success_t m_on_write_succeed;
    on_write_failed_t m_on_write_failed;
    on_read_failed_t m_on_read_failed;
    on_close_t m_on_close;
    server &m_server;
    tcp::socket m_socket;
    std::array<char, 25> m_data;
    char *m_body;
    char *m_buff_response;
    serialize::HeaderRequest m_current_header_request;
    serialize::Response m_current_response;
};

class server {
public:
    server(unsigned short port)
        : m_endpoint{ tcp::v4(), port }, m_acceptor{ m_service, m_endpoint } {};

    void run();
    void on_write_succeed(const on_write_success_t &onWriteSucceed);
    void on_write_failed(const on_write_failed_t &onWriteFaied);
    void on_read_succeed(const on_read_t &onReadSucceed);
    void on_read_failed(const on_read_failed_t &onReadFailed);
    void on_close(const on_close_t &onClose);
    void on_accept(const on_accept_t &onAccept);
    void stop();
private:
    void accept();
private:
    on_read_t m_on_read;
    on_write_success_t m_on_write_succeed;
    on_write_failed_t m_on_write_failed;
    on_read_failed_t m_on_read_failed;
    on_close_t m_on_close;
    on_accept_t m_on_accept;
    asio::io_service m_service;
    tcp::endpoint m_endpoint;
    tcp::acceptor m_acceptor;
    std::unique_ptr<InputConnection> m_waiting_connection;
    std::vector<std::unique_ptr<InputConnection>> m_connections{};
};

} // namespace protocol

#endif //SPACE_BATTLE_CONNECTIONLISTENER_HPP
