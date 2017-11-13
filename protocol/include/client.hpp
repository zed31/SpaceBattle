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
public:
    OutputConnection(asio::io_service &service);
    void close();
    void read();
    void write(const protocol::serialize::Request &request);
    void on_write_success(const std::function<void(OutputConnection &)> &onWriteSuccess);
    void on_write_failure(const std::function<void(std::error_code, OutputConnection &)> &onWriteFailure);
    void on_read_success(const std::function<void(const protocol::serialize::Response &, OutputConnection &)> &onReadSuccess);
    void on_read_failure(const std::function<void(std::error_code, OutputConnection&)> &onReadFailure);
    void on_close(const std::function<void(OutputConnection &)> &onClose);
private:
    std::function<void(OutputConnection &)> m_on_write_success;
    std::function<void(std::error_code, OutputConnection &)> m_on_write_failure;
    std::function<void(const protocol::serialize::Response &, OutputConnection &)> m_on_read_success;
    std::function<void(std::error_code, OutputConnection &)> m_on_read_failure;
    std::function<void(OutputConnection &)> m_on_close;
private:
    tcp::socket m_socket;
    std::string m_data_write;
    char *m_data;
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
