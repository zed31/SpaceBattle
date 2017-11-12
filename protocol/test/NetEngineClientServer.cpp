//
// Created by 49844 on 12/11/2017.
//

#include <functional>
#include <thread>
#include "asio.hpp"
#include "gtest/gtest.h"
#include "NetEngine.hpp"

using asio::ip::tcp;

class NetServerStub {
public:
    NetServerStub(unsigned limit = 1, unsigned limit_request_reiceived = 1)
        : m_request_reiceived{ limit_request_reiceived } {
        m_server_engine = std::make_unique<protocol::engine::NetEngineServer<NetServerStub>>(*this, static_cast<unsigned short>(6060), limit);
    }

    void run() {
        m_server_engine->run();
    }

    void on_request_read(const protocol::Request &request) {
        m_request_collection.push_back(request);
        --m_request_reiceived;
        std::cerr << "Read something, store it in the queue" << std::endl;
        /*if (m_request_reiceived == 0) {
            m_server_engine->stop();
        }*/
    }

    void on_accept(std::unique_ptr<protocol::engine::NetSessionServer<NetServerStub>> &&client_session) {
        client_session->read_request();
        m_connection_collection.push_back(std::move(client_session));
    }

    std::vector<protocol::Request> get_request() { return m_request_collection; };
private:
    std::vector<protocol::Request> m_request_collection;
    std::unique_ptr<protocol::engine::NetEngineServer<NetServerStub>> m_server_engine;
    std::vector<std::unique_ptr<protocol::engine::NetSessionServer<NetServerStub>>> m_connection_collection;
    unsigned m_request_reiceived;
};

class NetClientStub {
public:
    NetClientStub(std::string const &host, unsigned short port)
        : m_server_respond{ true } {
        m_client_engine = std::make_unique<protocol::engine::NetEngineClient<NetClientStub>>(host, port, *this);
    }

    void connect(std::function<void()> &on_session_ready) {
        m_on_session_ready = on_session_ready;
        m_client_engine->connect();
        m_is_running = true;
        while (m_is_running);
        std::cerr << "Client : Reiceived ending signal, closing connection now" << std::endl;
    }

    void on_response_read(const protocol::Response &) {
        std::cerr << "Reiceiving response from the server" << std::endl;
    }

    bool server_respond() const {
        return m_server_respond;
    }

    void write(const protocol::OpCode &opCode, const protocol::Body &body, std::function<void()> &on_write_complete) {
        std::cerr << "Writing through the network" << std::endl;
        m_on_write_complete = on_write_complete;
        m_client_session->write(opCode, body);
    }

    void on_connect(std::unique_ptr<protocol::engine::NetSessionClient<NetClientStub>> &&session) {
        m_client_session = std::move(session);
        m_client_session->read_response();
        m_on_session_ready();
    }

    void on_write_complete() {
        m_on_write_complete();
    }

    void close() {
        m_client_engine->stop();
        //m_client_session->close();
        m_is_running = false;
    }

    std::vector<protocol::Request> get_request() { return m_request_collection; };
private:
    std::vector<protocol::Request> m_request_collection;
    std::unique_ptr<protocol::engine::NetEngineClient<NetClientStub>> m_client_engine;
    std::unique_ptr<protocol::engine::NetSessionClient<NetClientStub>> m_client_session;
    std::function<void()> m_on_session_ready;
    std::function<void()> m_on_write_complete;
    bool m_server_respond;
    bool m_is_running;
};

static void run_client() {
    NetClientStub c("localhost", 6060);
    std::function<void()> f = [&] () {

        std::cerr << "Client : Connection complete, writing data through the network now" << std::endl;

        std::function<void()> f_write_complete = [&] () {

            std::cerr << "Client : Write complete closing connection now" << std::endl;
            c.close();

        };

        c.write(protocol::OpCode::CLIENT_DISCONNECT, protocol::Body{}, f_write_complete);
    };

    c.connect(f);
}

static std::vector<protocol::Request> run_server(unsigned limit = 1, unsigned limit_request = 1) {
    NetServerStub s{ limit, limit_request };
    s.run();
    return s.get_request();
}

TEST(Net_engine_client_s, connect_client_through_server) {
    std::thread t_server{[]() { run_server(); }};
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::thread t_client{[]() { run_client(); } };

    t_client.join();
    t_server.join();
}
