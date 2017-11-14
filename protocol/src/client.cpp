//
// Created by 49844 on 12/11/2017.
//

#include "client.hpp"

namespace protocol {

OutputConnection::OutputConnection(asio::io_service &io_service)
    : m_socket{ io_service } {}

void OutputConnection::on_write_success(const OutputConnection::on_write_success_t &onWriteSuccess) {
    m_on_write_success = onWriteSuccess;
}

void OutputConnection::on_write_failure(const OutputConnection::on_write_failure_t &onWriteFailure) {
    m_on_write_failure = onWriteFailure;
}

void OutputConnection::on_read_success(const OutputConnection::on_read_success_t &onReadSuccess) {
    m_on_read_success = onReadSuccess;
}

void OutputConnection::on_read_failure(const OutputConnection::on_read_failure_t &onReadFailure) {
    m_on_read_failure = onReadFailure;
}

void OutputConnection::on_close(const OutputConnection::on_close_t &onClose) {
    m_on_close = onClose;
}

void OutputConnection::read() {
    auto read_socket = [&, this] (std::error_code ec, std::size_t) {

        if (!ec) {

            serialize::HeaderResponse h{ m_data_header.data(), 25 };
            read_body(h);

        } else {

            if (m_on_read_failure) m_on_read_failure(ec, *this);

        }
    };
    asio::async_read(m_socket, asio::buffer(m_data_header.data(), 25), read_socket);
}

void OutputConnection::read_body(const serialize::HeaderResponse &header) {
    m_current_header_response = header;
    m_data_body = new char[header.bufferSize + 1];
    auto read_socket = [&, this] (std::error_code ec, std::size_t) {

        if (!ec) {

            serialize::Response resp{ m_current_header_response, serialize::Body{m_data_body, m_current_header_response.bufferSize} };
            if (m_on_read_success) m_on_read_success(resp, *this);

        } else {

            if (m_on_read_failure) m_on_read_failure(ec, *this);

        }

    };
    asio::async_read(m_socket, asio::buffer(m_data_body, header.bufferSize), read_socket);
}

void OutputConnection::write(const protocol::serialize::Request &request) {
    m_current_request = request;
    auto write_socket = [&, this] (std::error_code ec, std::size_t) {

        if (!ec) {

            if (m_on_write_success) m_on_write_success(*this, m_current_request);

        } else {

            if (m_on_write_failure) m_on_write_failure(ec, *this, m_current_request);

        }

    };
    m_data = new char[request.size()];
    request.data(m_data);
    asio::async_write(m_socket, asio::buffer(m_data, request.size()), write_socket);
};

void OutputConnection::close() {
    m_socket.shutdown(asio::ip::tcp::socket::shutdown_both);
    m_socket.close();
    if (m_on_close) m_on_close(*this);
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

void client::on_connection_failed(const std::function<void()> &onConnectionFailed) {
    m_on_connect_failed = onConnectionFailed;
}

void client::connect() {
    auto connect_socket = [&, this] (std::error_code ec, tcp::resolver::iterator) {
        if (!ec) {
            if (m_on_connect) m_on_connect(m_connection);
        } else {
            if (m_on_connect_failed) m_on_connect_failed();
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
