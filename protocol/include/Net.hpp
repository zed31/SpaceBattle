//
// Created by 49844 on 04/11/2017.
//

#ifndef SPACE_BATTLE_NET_HPP
#define SPACE_BATTLE_NET_HPP

#include "asio.hpp"

namespace protocol {

using asio::ip::tcp;

/*! \brief This class is an interface for the communication between client and server
 * It is implemented below by NetIOClient and NetIOServer
*/
class Net {
public:
    /*! \brief The constructor of the Net class
     * the socket parameter is moved during the construction of the
     * NetIO class
     * @param[in] io_service    The io_service used by the client / server
    */
    Net(asio::io_service &io_service);
protected:
    asio::io_service *m_io_service;
};

/*! \brief This class is used to create a simple server connection that run the io_service and accept clients
 * It inherits from a Net class.
*/
class NetServer : public Net {
public:
    /*! \brief Constructor of the NetServer
     * Construct the Net base class, and construct the acceptor to start accepting
     * clients. The io_service is moved during the construction
     * @param[in]   io_service  The io_service used by the server to run the event loop
     * @param[in]   endpoint    The endpoint used to connect to them
    */
    NetServer(asio::io_service &io_service, const tcp::endpoint &endpoint);

    /*! \brief accept the connection from client and server
     * This function accept a connection between the client and the server, the callback is called when a client
     * is connected to the server.
     * @param[in]   connection_callback Callback called when a new connection is triggered by the io_service
     * this callback takes a socket as parameter, to handle the connection the way the user wants. This socket is also
     * moved inside the callback.
    */
    template<typename CallbackSuccess>
    void accept(CallbackSuccess &&success_callback) {
        m_acceptor.async_accept(m_socket, [&, this] (std::error_code ec) {
            if (!ec) {
                success_callback(std::move(m_socket));
            }
            accept(success_callback);
        });
    }
private:
    tcp::acceptor m_acceptor;
    tcp::socket m_socket;
};

/*! \brief This class is used to handle client connection
 * It inherits from a Net class
*/
class NetClient : public Net {
public:
    /*! \brief Constructor for NetClient
     * Construct the Net base class and the socket
     * @param[in]   io_service          the io_service used by the client to run the event loop
     * @param[in]   endpoint_iterator   the iterator to the first endpoint
    */
    NetClient(asio::io_service &io_service, tcp::resolver::iterator endpoint_iterator);

    /*! \brief Connect to a specific endpoint list
     * @param[in]   callback_connection the callback that is used when the connection succeed. This callback
     * takes a socket moved as parameter.
     * @param[in]   callback_failure    the callback that is used when the connection failed. this callback
     * takes the std::error_code as argument.
    */
    template<typename CallbackSuccess, typename CallbackFailure>
    void connect(CallbackSuccess &&connection_success, CallbackFailure &&connection_failure) {
        asio::async_connect(m_socket, m_endpoint_iterator, [&, this] (std::error_code ec, tcp::resolver::iterator) {
            if (!ec) {
                connection_success(std::move(m_socket));
            } else {
                std::cout << "failure" << std::endl;
                connection_failure(ec);
            }
        });
    };

    /*! \brief Close the client communication
     * This function is used to close the socket.
    */
    void close();
private:
    tcp::resolver::iterator m_endpoint_iterator;
    tcp::socket m_socket;
};

} // namespace protocol

#endif //SPACE_BATTLE_NET_HPP
