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
#include "serialization/response.hpp"
#include "serialization/request.hpp"

using asio::ip::tcp;

namespace protocol {

class OutputConnection{
    friend class client;
    using on_write_success_t = std::function<void(OutputConnection &, const serialize::Request &)>;
    using on_read_success_t = std::function<void(const protocol::serialize::Response &, OutputConnection &)>;
    using on_write_failure_t = std::function<void(std::error_code, OutputConnection &, const serialize::Request &)>;
    using on_read_failure_t = std::function<void(std::error_code, OutputConnection&)>;
    using on_close_t = std::function<void(OutputConnection &)>;
public:
    OutputConnection(asio::io_service &service);
    void close();
    void read();
    void write(const protocol::serialize::Request &request);
    void on_write_success(const on_write_success_t &onWriteSuccess);
    void on_write_failure(const on_write_failure_t &onWriteFailure);
    void on_read_success(const on_read_success_t &onReadSuccess);
    void on_read_failure(const on_read_failure_t &onReadFailure);
    void on_close(const on_close_t &onClose);
private:
    void read_body(const serialize::HeaderResponse &header);
    on_write_success_t m_on_write_success;
    on_read_success_t m_on_read_success;
    on_write_failure_t m_on_write_failure;
    on_read_failure_t m_on_read_failure;
    on_close_t m_on_close;
private:
    tcp::socket m_socket;
    std::string m_data_write;
    char *m_data;
    char *m_data_body;
    std::array<char, 25> m_data_header;
    serialize::Request m_current_request;
    serialize::HeaderResponse m_current_header_response;
};

class client {
public:
    client(const std::string &host, const std::string &port);
    void connect();
    void run();
    void on_connect(const std::function<void(OutputConnection&)> &onConnect);
    void stop();
private:
    std::function<void(OutputConnection&)> m_on_connect;

    asio::io_service m_service;
    asio::io_service::work m_service_worker;
    OutputConnection m_connection;
    tcp::resolver m_resolver;
    std::string m_host;
    std::string m_port;
};

} // namespace protocol

#endif //SPACE_BATTLE_CLIENT_HPP
