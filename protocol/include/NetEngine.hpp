//
// Created by 49844 on 04/11/2017.
//

#ifndef SPACE_BATTLE_NETENGINE_H
#define SPACE_BATTLE_NETENGINE_H

#include <mutex>
#include <string>
#include <iostream>
#include <memory>
#include "Net.hpp"
#include "NetIO.hpp"

namespace protocol {

namespace engine {

template<
    typename NetConnector,
    typename NetSession,
    typename Engine
>
class NetEngine {};

template<typename Engine>
class NetSessionServer {
    using io_input = protocol::io::NetIO<protocol::io::IoType::INPUT, NetSessionServer<Engine>>;
    using io_output = protocol::io::NetIO<protocol::io::IoType::OUTPUT, NetSessionServer<Engine>>;
    using ptr_net_io_input = std::unique_ptr<io_input>;
    using ptr_net_io_output = std::unique_ptr<io_output>;
    using ptr_sock = std::unique_ptr<tcp::socket>;

    friend class protocol::io::NetIO<protocol::io::IoType::INPUT, NetSessionServer<Engine>>;
    friend class protocol::io::NetIO<protocol::io::IoType::OUTPUT, NetSessionServer<Engine>>;
public:
    /*! \brief Construct the session with the socket and the engine
     * @param[in]   sock    A pointer to a socket used to handle the IO
     * @param[in]   engine  The engine called when IO operation success
    */
    NetSessionServer(ptr_sock &&sock, Engine *engine)
        : m_socket{ std::move(sock) }, m_io_input{ std::make_unique<io_input>(m_socket.get(), *this) },
          m_io_output{ std::make_unique<io_output>(m_socket.get(), *this) }, m_engine{ engine } {}

    /*! \brief Read a request to the stream and call the onReadSuccess from the engine passed to the construction */
    void read_request() {
        std::cout << "Reading request from the client..." << std::endl;
        m_io_output->read();
    }

    /*! \brief write response into the stream
     * @param[in]   statusCode  The statusCode of the response
     * @param[in]   body        The body of the response
    */
    void write(const protocol::StatusCode &opCode, const protocol::Body &body) {
        protocol::HeaderResponse h{ protocol::HeaderUtility::default_magic_, protocol::HeaderUtility::default_rev_, opCode, body.size() };
        protocol::Response r{h, body};
        m_data = new char[r.size()];
        r.data(m_data);
        m_io_input->write(m_data, r.size());
    }
private:
    void success(const protocol::Request &request) {
        std::cerr << "Server : Reicieved something from the client" << std::endl;
        m_engine->on_request_read(request);
    }
    void success(const protocol::Response &) {
        std::cout << "Reicieved response" << std::endl;
        //m_engine->on_request_read(request);
    }

    void on_read_failure(const std::error_code &ec) {
        std::cerr << "Error in server session : " << ec.message() << std::endl;
        m_socket->close();
    }
private:
    ptr_sock m_socket;
    ptr_net_io_input m_io_input;
    ptr_net_io_output m_io_output;
    Engine *m_engine;
    char *m_data;
};

template<typename Engine>
class NetEngine<
    protocol::NetServer,
    NetSessionServer<Engine>,
    Engine
> {
    using ptr_net_connector = std::unique_ptr<protocol::NetServer>;
public:
    /*! \brief constuct server engine with the limit of client possible to connect on it
     * @param[in]   engine  The engine called when the IO operations are performed
     * @param[in]   port    The port used to connect to a server
     * @param[in]   limit   the limit of client that can be accepted inside the server
    */
    NetEngine(Engine &engine, unsigned short port, unsigned limit = 1) : m_limit{limit}, m_engine{ &engine }, m_net_server{ nullptr } {
        tcp::endpoint edp{ tcp::v4(), port };
        m_net_server = std::make_unique<protocol::NetServer>(m_service, edp, limit);
    };

    /*! \brief run the acceptor and call the callback when a connection occur
     * It connects client and create a new session with an input and output, it also move the socket
     * created and pass it as constructor of the NetSession. The NetSession start to read the Request
     * right after its creation
    */
    void run() {
        m_net_server->accept([this](auto &&socket_ptr){
            std::cerr << "Server: New connection, listen the socket now..." << std::endl;
            auto connection = std::make_unique<NetSessionServer<Engine>>(std::move(socket_ptr), m_engine);
            m_engine->on_accept(std::move(connection));
        });
        m_service.run();
        std::cout << "Service ending, closing the server..." << std::endl;
    };

    /*! \brief Used to stop the service */
    void stop() {
        std::cerr << "Stopping the server" << std::endl;
        m_service.stop();
    }
private:
    asio::io_service m_service;
    unsigned m_limit;
    Engine *m_engine;
    ptr_net_connector m_net_server;
};

template<typename Engine>
class NetSessionClient {
    using io_input = protocol::io::NetIO<protocol::io::IoType::INPUT, NetSessionClient<Engine>>;
    using io_output = protocol::io::NetIO<protocol::io::IoType::OUTPUT, NetSessionClient<Engine>>;
    using ptr_net_io_input = std::unique_ptr<io_input>;
    using ptr_net_io_output = std::unique_ptr<io_output>;
    using ptr_sock = std::unique_ptr<tcp::socket>;

    friend class protocol::io::NetIO<protocol::io::IoType::INPUT, NetSessionClient<Engine>>;
    friend class protocol::io::NetIO<protocol::io::IoType::OUTPUT, NetSessionClient<Engine>>;
public:
    /*! \brief Construct the client
     * @param[in]   socket_ptr  The stream connection used to perform IO operations on the server
     * @param[in]   engine      The engine used to send data to the client
    */
    NetSessionClient(ptr_sock &&socket_ptr, Engine *engine)
        : m_socket{ std::move(socket_ptr) }, m_engine{ engine },
          m_io_input{ std::make_unique<io_input>(m_socket.get(), *this) }, m_io_output{ std::make_unique<io_output>(m_socket.get(), *this) } {};

    /*! \brief perform IO read and return to the engine the response received
     * The method on_response_receive() should be called with the response as parameter.
    */
    void read_response() {
        std::cerr << "Server : Reading response from the server..." << std::endl;
        m_io_output->read_response();
    };

    /*! \brief write request into the stream
     * @param[in]   opCode  The opCode of the request
     * @param[in]   body    The body of the request
    */
    void write(const protocol::OpCode &opCode, const protocol::Body &body) {
        protocol::HeaderRequest h{ protocol::HeaderUtility::default_magic_, protocol::HeaderUtility::default_rev_, opCode, body.size() };
        protocol::Request r{h, body};
        m_data = new char[r.size()];
        r.data(m_data);
        if (m_io_input) {
            m_io_input->write(m_data, r.size());
        }
    }

    /*! \brief Close the stream */
    void close() {
        m_socket->close();
    }
private:
    void success(const protocol::Response &response) {
        std::cerr << "Response recovered from the server..." << std::endl;
        m_engine->on_response_read(response);
    }
    void success(const protocol::Request &) {
        std::cerr << "Request recovered from the server..." << std::endl;
    }
    void on_read_failure(const std::error_code &ec) {
        std::cerr << "Client : Failed when reading on the server : " << ec.message() << std::endl;
    }
    void on_write_success() {
        std::cerr << "Client : Writing through the network success" << std::endl;
        m_engine->on_write_complete();
    }
    void on_write_failed(const std::error_code& e) {
        std::cerr << "error in connect : " << e.message() << std::endl;
    }
private:
    ptr_sock m_socket;
    Engine *m_engine;
    ptr_net_io_input m_io_input;
    ptr_net_io_output m_io_output;
    char *m_data;
};

template<typename Engine>
class NetEngine<protocol::NetClient, NetSessionClient<Engine>, Engine> {
    using client_connector = protocol::NetClient;
    using ptr_net_client = std::unique_ptr<protocol::NetClient>;
public:
    /*! \brief Construct the NetEngine
     * @param[in]   host    The host to connect to
     * @param[in]   port    The port used to connect to
     * @param[in]   engine  The engine used to called when an IO operation is performed
    */
    NetEngine(const std::string &host, unsigned short port, Engine &engine)
        : m_worker_service{ m_service }, m_engine{ &engine }, m_resolver{ m_service }, m_service_startup{ nullptr }, m_connector{ nullptr }  {
        auto endpoint_iterator = m_resolver.resolve({host, std::to_string(port)});
        m_connector = std::make_unique<client_connector>(m_service, endpoint_iterator);
    }

    /*! \brief Connect a client to a specific endpoint */
    void connect() {
        m_connector->connect([this](auto &&socket_ptr) {
            std::cerr << "New client connected to the server..." << std::endl;
            auto session = std::make_unique<NetSessionClient<Engine>>(std::move(socket_ptr), m_engine);
            m_engine->on_connect(std::move(session));
        }, [](auto){ std::cerr << "Failed to connect" << std::endl; });
        m_service_startup = std::make_unique<std::thread>([this]() { m_service.run(); });
    }

    void stop() {
        m_service.stop();
    }
private:
    asio::io_service m_service;
    asio::io_service::work m_worker_service;
    tcp::resolver m_resolver;
    Engine *m_engine;
    std::unique_ptr<std::thread> m_service_startup;
    ptr_net_client m_connector;
};

template<typename Engine>
using NetEngineServer = NetEngine<protocol::NetServer, NetSessionServer<Engine>, Engine>;

template<typename Engine>
using NetEngineClient = NetEngine<protocol::NetClient, NetSessionClient<Engine>, Engine>;

} // namespace engine

} // namespace protocol

#endif //SPACE_BATTLE_NETENGINE_H
