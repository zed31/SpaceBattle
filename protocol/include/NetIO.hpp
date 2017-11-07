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
template<IoType IO_TYPE>
class NetIO;

template<>
class NetIO<IoType::INPUT> {
public:
    /*! \brief Construction of the NetIO input specialization
     * @param[in]   io_service  The polling system that run the loop and triggered the events
     * @param[in]   socket      The socket used to write data on it
    */
    NetIO(asio::io_service &io_service, tcp::socket &socket) : m_io_service{ &io_service }, m_socket{ &socket } {};

    /*! \brief Post method used to execute the callback when the MessageQueue is empty
     * This function should be called before the write, to run the write function inside the thread that run the
     * io_service.
     * @param[in]   success     Callback called when the message queue is empty, it takes no parameter
    */
    template<typename CallbackSuccess>
    void post(CallbackSuccess &&success) {
        m_io_service->post([&success, this] () {
            bool writeInProgress = !m_request.empty();
            if (!writeInProgress) {
                success();
            }
        });
    }

    /*! \brief Write the message inside the queue, then sent it through the network
     * @param[in]   message     The message request
     * @param[in]   success     The callback called when the write end
     * @param[in]   failure     The callback called when an error occur, it takes the std::error_code as parameter
    */
    template<typename CallbackSuccess, typename CallbackFailure>
    void write(const protocol::Request &message, CallbackSuccess &&success, CallbackFailure &&failure) {
        auto *data = new char[message.size()];
        message.data(data);
        asio::async_write(*m_socket, asio::buffer(data, message.size()), [&, this] (const std::error_code &ec, std::size_t) {
            delete[] data;
            if (!ec) {
                protocol::Request msg = m_request.front();
                m_request.erase(m_request.begin());
                if (!m_request.empty()) {
                    write(msg, success, failure);
                } else {
                    success();
                }
            } else {
                failure(ec);
            }
        });
    };
private:
    MessageQueue m_request;
    asio::io_service *m_io_service;
    tcp::socket *m_socket;
};

template<>
class NetIO<IoType::OUTPUT> {
public:
    /*! \brief Construct the NetIO output class used to read data through network
     * @param[in]   socket  The socket used to read data
    */
    NetIO(tcp::socket &socket) : m_socket{ &socket } {};

    /*! \brief read data through the network
     * @param[in]   success     The callback used when the read finish without errors, it takes a protocol::Response
     * as parameter
     * @param[in]   failure     The callback used when the read fail at some point, it contains an std::error_code
     * containing the error
    */
    template<typename CallbackSuccess, typename CallbackFailure>
    void read(CallbackSuccess &&success, CallbackFailure &&failure) {
        read_header(success, failure);
    };
private:
    template<typename CallbackSuccess, typename CallbackFailure>
    void read_header(CallbackSuccess &&success, CallbackFailure &&failure) {
        auto *data = new char[25];
        asio::async_read(*m_socket, asio::buffer(data, 25), [&, this] (const std::error_code &ec, std::size_t) {
            if (!ec) {
                protocol::HeaderResponse h{ data, 25 };
                delete[] data;
                read_body(h, success, failure);
            } else {
                failure(ec);
            }
        });
    }

    template<typename CallbackSuccess, typename CallbackFailure>
    void read_body(const protocol::HeaderResponse &header, CallbackSuccess &&success, CallbackFailure &&failure) {
        auto *data = new char[header.bufferSize];
        asio::async_read(*m_socket, asio::buffer(data, header.bufferSize), [&, this] (const std::error_code &ec, std::size_t) {
            if (!ec) {
                protocol::Body b{ data, header.bufferSize };
                delete[] data;
                success(protocol::Response{ header, b });
            } else {
                failure(ec);
            }
        });
    };

    tcp::socket *m_socket;
};

} // namesapce io

} // namespace protocol

#endif //SPACE_BATTLE_NETIO_HPP
