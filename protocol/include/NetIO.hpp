//
// Created by 49844 on 06/11/2017.
//

#ifndef SPACE_BATTLE_NETIO_HPP
#define SPACE_BATTLE_NETIO_HPP

#include <vector>
#include "NetIOBusinessData.hpp"

namespace protocol {

namespace io {

enum class IoType { INPUT, OUTPUT };

using MessageQueue = std::vector<protocol::Request>;

/*! \brief Class used to send and receive messages from the client and the server */
template<IoType IO_TYPE, typename Engine>
class NetIO;

template<typename Engine>
class NetIO<IoType::INPUT, Engine> {
public:
    /*! \brief Construction of the NetIO input specialization
     * @param[in]   socket      The socket used to write data on it
     * @param[in]   engine      The engine used to call when a IO operation is done
    */
    NetIO(tcp::socket *socket, Engine &engine) : m_socket{ socket }, m_engine{ &engine }, m_data{0} {};

    /*! \brief Write the message inside the queue, then sent it through the network
     * @param[in]   message     The message request
     * @param[in]   size        The size of the message
    */
    void write(const char *data, const std::size_t size) {
        m_data = new char[size];
        for (unsigned i = 0; i < size; ++i) {
            m_data[i] = data[i];
        }
        asio::async_write(*m_socket, asio::buffer(m_data, size), [&, this] (const std::error_code &ec, std::size_t) {
            if (!ec) {
                delete[] m_data;
                m_engine->on_write_success();
            } else {
                m_engine->on_write_failed(ec);
            }
        });
    };
private:
    MessageQueue m_request;
    tcp::socket *m_socket;
    char *m_data;
    Engine *m_engine;
};

template<typename Engine>
class NetIO<IoType::OUTPUT, Engine> {
public:
    /*! \brief Construct the NetIO output class used to read data through network
     * @param[in]   socket  The socket used to read data
    */
    NetIO(tcp::socket *socket, Engine& e) : m_socket{ socket }, m_engine{ &e } {};

    /*! \brief read request through the network
    */
    void read() {
        m_socket->async_read_some(asio::buffer(m_data_arr.data(), 25), [&, this] (const std::error_code &ec, std::size_t) {
            if (!ec) {
                protocol::HeaderRequest h{ m_data_arr.data(), 25 };
                read_body(h);
            } else {
                m_engine->on_read_failure(ec);
            }
        });
    };

    /*! \brief read response through the network */
    void read_response() {
        read_header_response();
    }
private:
    void read_header_response() {
        m_socket->async_read_some(asio::buffer(m_data_arr.data(), 25), [&, this] (const std::error_code &ec, std::size_t) {
            if (!ec) {
                protocol::HeaderResponse h{ m_data_arr.data(), 25 };
                read_body_response(h);
            } else {
                m_engine->on_read_failure(ec);
            }
        });
    }

    void read_body(const protocol::HeaderRequest &header) {
        m_data_body = new char[header.bufferSize + 1];
        m_header_read = header;
        asio::async_read(*m_socket, asio::buffer(m_data_body, m_header_read.bufferSize), [&, this] (const std::error_code &ec, std::size_t) {
            if (!ec) {
                protocol::Body b{ m_data_body, m_header_read.bufferSize };
                delete[] m_data_body;
                m_engine->success(protocol::Request{ m_header_read, b });
                read();
            } else {
                m_engine->on_read_failure(ec);
            }
        });
    };

    void read_body_response(const protocol::HeaderResponse &header) {
        m_data_body = new char[header.bufferSize + 1];
        m_header_response = header;
        asio::async_read(*m_socket, asio::buffer(m_data_body, m_header_response.bufferSize), [&, this] (const std::error_code &ec, std::size_t) {
            if (!ec) {
                protocol::Body b{ m_data_body, m_header_response.bufferSize };
                delete[] m_data_body;
                m_engine->success(protocol::Response{ m_header_response, b });
            } else {
                m_engine->on_read_failure(ec);
            }
        });
    }

    tcp::socket *m_socket;
    std::array<char, 25> m_data_arr{};
    char *m_data_body;
    protocol::HeaderRequest m_header_read;
    protocol::HeaderResponse m_header_response;
    Engine *m_engine;
};

} // namesapce io

} // namespace protocol

#endif //SPACE_BATTLE_NETIO_HPP
