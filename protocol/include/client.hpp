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

/*! \brief Used to handle the IO operation on the network */
class OutputConnection{
    friend class client;
    using on_write_success_t = std::function<void(OutputConnection &, const serialize::Request &)>;
    using on_read_success_t = std::function<void(const protocol::serialize::Response &, OutputConnection &)>;
    using on_write_failure_t = std::function<void(std::error_code, OutputConnection &, const serialize::Request &)>;
    using on_read_failure_t = std::function<void(std::error_code, OutputConnection&)>;
    using on_close_t = std::function<void(OutputConnection &)>;
public:
    /*! \brief Construct the outputConnection
     * The OutputConnection allows the user to make operation on socket
     * @param[in]   service The service used to construct the socket
    */
    OutputConnection(asio::io_service &service);

    /*! \brief Close the socket
     * Close the stream, and force the shutdown of the read and write operation
    */
    void close();

    /*! \brief Read the response from the server
     * Read the response from the server then serialize it into a Response object and call the
     * user-defined on_read_success() user-defined function if read succeed, on_read_failed() otherwise
    */
    void read();

    /*! \brief Write the request on the stream
     * Write the request on the stream, call on_write_success() user-defined function if write succeed,
     * on_write_failed() user-defined otherwise
     * @param[in]   request The request sent to the server
    */
    void write(const protocol::serialize::Request &request);

    /*! \brief Set the on_write_success() user-defined function
     * @param[in]   onWriteSuccess  Function write if the write success, this object is an std::function<void(OutputConnection &, const serialize::Request &)>
    */
    void on_write_success(const on_write_success_t &onWriteSuccess);

    /*! \brief Set the on_write_failure() user-defined function
     * @param[in]   onWriteFailure  Function write if the write fails, this object is an std::function<void(std::error_code, OutputConnection &, const serialize::Request &)>
    */
    void on_write_failure(const on_write_failure_t &onWriteFailure);

    /*! \brief Set the on_read_success() user-defined function
     * @param[in]   onReadSuccess   Function read if the read success, this object is an std::function<void(const protocol::serialize::Response &, OutputConnection &)>
    */
    void on_read_success(const on_read_success_t &onReadSuccess);

    /*! \brief Set the on_read_failure() user-defined function
     * @param[in]   onReadFailure   Function read if the read fails, this object is an std::function<void(std::error_code, OutputConnection&)>;
    */
    void on_read_failure(const on_read_failure_t &onReadFailure);

    /*! \brief Set the on_close() user-defined function
     * @param[in]   onClose Function close if a close happens on the socket.
    */
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
    //std::string m_data_write;
    char *m_data;
    char *m_data_body;
    std::array<char, 25> m_data_header;
    serialize::Request m_current_request;
    serialize::HeaderResponse m_current_header_response;
};

/*! \brief Used to make the connection to the server */
class client {
public:
    /*! \brief Create a client
     * @param[in]   host    The host where the client will connect when the connect() would be called
     * @param[in]   port    The port where the client will connect when the connect() would be called
    */
    client(const std::string &host, const std::string &port);

    /*! \brief Connect a client
     * Call the on_connect() user-definec function when the connection is done, and on_connect_failure() when
     * the connection fails
    */
    void connect();

    /*! \brief run the service and connect the client
     * Run the service to handle events on stream and then connect the client
    */
    void run();

    /*! \brief set-up the on_connect() user-defined function
     * @param[in]   onConnect   The function used to be called when a client is connected, it has the form : std::function<void(OutputConnection&)>
    */
    void on_connect(const std::function<void(OutputConnection&)> &onConnect);

    /*! \brief set-up the on_connect_failed() user-defined function
     * @param[in]   onConnectionFailed  The function used to be called when a connection fails, it has the form : std::function<void()>
    */
    void on_connection_failed(const std::function<void()> &onConnectionFailed);

    /*! \brief stop the service */
    void stop();
private:
    std::function<void(OutputConnection&)> m_on_connect;
    std::function<void()> m_on_connect_failed;

    asio::io_service m_service;
    asio::io_service::work m_service_worker;
    OutputConnection m_connection;
    tcp::resolver m_resolver;
    std::string m_host;
    std::string m_port;
};

} // namespace protocol

#endif //SPACE_BATTLE_CLIENT_HPP
