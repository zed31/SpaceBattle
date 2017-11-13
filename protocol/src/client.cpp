//
// Created by 49844 on 12/11/2017.
//

#include <iostream>
#include "client.hpp"

namespace protocol {

OutputConnection::OutputConnection(asio::io_service &io_service)
    : m_socket{ io_service } {}

void OutputConnection::on_write_success(const std::function<void(OutputConnection &)> &onWriteSuccess) {
    m_on_write_success = onWriteSuccess;
}

void OutputConnection::on_write_failure(const std::function<void(std::error_code, OutputConnection &)> &onWriteFailure) {
    m_on_write_failure = onWriteFailure;
}

void OutputConnection::on_read_success(const std::function<void(const protocol::serialize::Response &, OutputConnection&)> &onReadSuccess) {
    m_on_read_success = onReadSuccess;
}

void OutputConnection::on_read_failure(const std::function<void(std::error_code, OutputConnection&)> &onReadFailure) {
    m_on_read_failure = onReadFailure;
}

void OutputConnection::on_close(const std::function<void(OutputConnection &)> &onClose) {
    m_on_close = onClose;
}

void OutputConnection::read() {
    auto read_socket = [&, this] (std::error_code ec, std::size_t length) {

        std::cout << "OutputConnection::read " << length << "bytes" << std::endl;
        if (!ec) {

            std::cerr << "OutputConnection::read : Read success" << std::endl;
            std::cerr << "OutputConnection::read : " << m_data << std::endl;
            //TODO : Insert on_read_success here
            read();

        } else {

            std::cerr << "OutputConnection::read error : " << ec.message() << std::endl;
            if (m_on_read_failure) m_on_read_failure(ec, *this);

        }
    };
    asio::async_read(m_socket, asio::buffer(m_data, 25), read_socket);
}

void OutputConnection::write(const protocol::serialize::Request &request) {
    auto write_socket = [&, this] (std::error_code ec, std::size_t length) {

        std::cerr << "Writing " << length << " datas" << std::endl;
        std::cerr << m_data << std::endl;
        if (!ec) {

            if (m_on_write_success) m_on_write_success(*this);

        } else {

            std::cerr << "Error : " << ec.message() << std::endl;
            if (m_on_write_failure) m_on_write_failure(ec, *this);

        }

    };
    m_data = new char[request.size()];
    request.data(m_data);
    asio::async_write(m_socket, asio::buffer(m_data, request.size()), write_socket);
};

void OutputConnection::close() {
    m_socket.shutdown(asio::ip::tcp::socket::shutdown_both);
    m_socket.close();
    //TODO : Insert on_close here
}


} // namespace protocol

namespace protocol {

client::client(const std::string &host, const std::string &port)
    : m_service_worker{m_service}, m_connection{ m_service }, m_resolver{ m_service },
      m_host{ host }, m_port{ port } {
}

void client::on_connect(const std::function<void(OutputConnection&)> &onConnect) {
    m_on_connect = onConnect;
}

void client::connect() {
    auto connect_socket = [&, this] (std::error_code ec, tcp::resolver::iterator) {
        if (!ec) {
            std::cout << "client::connect Connection complete" << std::endl;
            if (m_on_connect) m_on_connect(m_connection);
        } else {
            std::cerr << "client::connect Error on connection : " << ec.message() << std::endl;
        }
    };
    auto endpoint_iterator = m_resolver.resolve({m_host, m_port});
    asio::async_connect(m_connection.m_socket, endpoint_iterator, connect_socket);
}

void client::run() {
    connect();
    m_service.run();
}

void client::stop() {
    m_service.stop();
}

} // namespace protocol
