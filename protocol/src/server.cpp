//
// Created by 49844 on 12/11/2017.
//

#include "server.hpp"

namespace protocol {
    void InputConnection::read() {

        auto read_socket = [&, this](std::error_code ec, std::size_t length) {
            std::cout << "Reading something of " << length << std::endl;
            if (!ec) {

                std::cout << m_data.data() << std::endl;
                std::cerr << "Sending back the datas" << std::endl;
                read_body(serialize::HeaderRequest{ m_data.data(), m_data.size() });

            } else {

                if (m_on_read_failed) m_on_read_failed(ec, *this);
                std::cerr << "Error " << ec.message() << std::endl;

            }
        };

        asio::async_read(m_socket, asio::buffer(m_data, 25), read_socket);
    }

    void InputConnection::read_body(const serialize::HeaderRequest &requestHeader) {
        m_current_header_request = requestHeader;
        m_body = new char[requestHeader.bufferSize];

        auto read_socket = [&, this] (std::error_code ec, std::size_t) {

            if (!ec) {

                std::cerr << "InputConnection::read_body : Success" << std::endl;
                if (m_on_read) m_on_read(serialize::Request{requestHeader, serialize::Body{m_body, m_current_header_request.bufferSize}}, *this);

            } else {

                std::cerr << "InputConnection::read_body : Failed" << std::endl;
                if (m_on_read_failed) m_on_read_failed(ec, *this);

            }
        };

        asio::async_read(m_socket, asio::buffer(m_body, requestHeader.bufferSize), read_socket);
    }

    void InputConnection::close() {
        m_socket.shutdown(tcp::socket::shutdown_both);
        m_socket.close();
        if (m_on_close) m_on_close();
    }

    void InputConnection::on_accept() {
        std::cout << "Start reading datas" << std::endl;
        read();
    }

    void InputConnection::write(const serialize::Response &response) {
        m_buff_response = new char[response.size()];
        response.data(m_buff_response);
        m_current_response = response;
        auto write_socket = [&, this](std::error_code ec, std::size_t length) {
            std::cout << "writing something of " << length << std::endl;

            if (!ec) {

                std::cerr << "Write complete !" << std::endl;
                if (m_on_write_succeed) m_on_write_succeed(m_current_response, *this);

            } else {

                std::cerr << "Error " << ec.message() << std::endl;
                if (m_on_write_failed) m_on_write_failed(ec, *this);

            }
        };
        asio::async_write(m_socket, asio::buffer(m_buff_response, response.size()), write_socket);
    }
}

namespace protocol {
    void server::accept() {
        m_waiting_connection = std::make_unique<InputConnection>(*this, m_service);
        m_waiting_connection->m_on_read = m_on_read;
        m_waiting_connection->m_on_write_succeed = m_on_write_succeed;
        m_waiting_connection->m_on_write_failed = m_on_write_failed;
        m_waiting_connection->m_on_read_failed = m_on_read_failed;
        m_waiting_connection->m_on_close = m_on_close;
        auto accept_complete = [&, this] (std::error_code ec) {
            if (!ec) {
                std::cout << "New connection occur ..." << std::endl;
                m_waiting_connection->on_accept();
                m_connections.push_back(std::move(m_waiting_connection));
                accept();
            }
        };
        m_acceptor.async_accept(m_waiting_connection->m_socket, accept_complete);
    }

    void server::on_read_failed(const on_read_failed_t &onReadFailed) {
        m_on_read_failed = onReadFailed;
    }

    void server::on_read_succeed(const on_read_t &onReadSucceed) {
        m_on_read = onReadSucceed;
    }

    void server::on_write_failed(const on_write_failed_t &onWriteFailed) {
        m_on_write_failed = onWriteFailed;
    }

    void server::on_write_succeed(const on_write_success_t &onWriteSucceed) {
        m_on_write_succeed = onWriteSucceed;
    }

    void server::on_close(const on_close_t &onClose) {
        m_on_close = onClose;
    }

    void server::run() {
        accept();
        m_service.run();
    }

    void server::stop() {
        m_service.stop();
    }
}