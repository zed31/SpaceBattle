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
using on_accept_t = std::function<void(std::unique_ptr<InputConnection> &&)>;

/*! \brief Class used to send and receive message from the clients */
class InputConnection {
    friend class server;
public:
    /*! \brief Construct the socket thanks to the service
     * @param[in]   server  The server used to accept clients
     * @param[in]   service The service used to create the socket
    */
    InputConnection(server &server, asio::io_service &service)
        : m_server{ server }, m_socket{ service } {};
    /*! \brief shutdown the connection and destroy the associated connection */
    ~InputConnection();

    void write(const serialize::Response &response);

    /*! \brief Bind the on_write_succeed() function to a specific user-defined function
     * @param[in]   onWriteSucceed  function under the form of std::function<void(const serialize::Response &, InputConnection &)>;
    */
    void on_write_succeed(const on_write_success_t &onWriteSucceed);

    /*! \brief Bind the on_write_failed() function to a specific user-defined function
     * @param[in]   onWriteFaied    function under the form of std::function<void(std::error_code, InputConnection &)>;
    */
    void on_write_failed(const on_write_failed_t &onWriteFaied);

    /*! \brief Bind the on_read_succeed() function to a specific user-defined function
     * @param[in]   onReadSucceed   function under the form of std::function<void(const serialize::Request &, InputConnection &)>;
    */
    void on_read_succeed(const on_read_t &onReadSucceed);

    /*! \brief Bind the on_read_failed() function to a specific user-defined function
     * @param[in]   onReadFailed    function under the form of std::function<void(std::error_code, InputConnection &)>;
    */
    void on_read_failed(const on_read_failed_t &onReadFailed);

    /*! \brief Bind the on_close() function to a specific user-defined function
     * @param[in]   onClose function under the form of std::function<void()>;
    */
    void on_close(const on_close_t &onClose);

    void read();
private:
    void on_accept();
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
    /*! \brief Construct the server but not run it !
     * @param[in]   port    Port to bind it
    */
    server(unsigned short port)
        : m_endpoint{ tcp::v4(), port }, m_acceptor{ m_service, m_endpoint } {};

    /*! \brief run the service and start accept connection
     * It creates a blocking-accept until the service stop
    */
    void run();

    /*! \brief stop the service */
    void stop();


    /*! \brief Bind the on_accept() function to a specific user-defined function
     * @param[in]   onAccept    function under the form of std::function<void(std::unique_ptr<InputConnection> &&)>;
    */
    void on_accept(const on_accept_t &onAccept);
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
};

} // namespace protocol

#endif //SPACE_BATTLE_CONNECTIONLISTENER_HPP
