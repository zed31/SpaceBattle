//
// Created by 49844 on 11/11/2017.
//

#include <thread>
#include "asio.hpp"
#include "gtest/gtest.h"
#include "NetEngine.hpp"

/*using asio::ip::tcp;

class NetServerStub {
public:
    NetServerStub(unsigned limit = 1, unsigned limit_request_reiceived = 1) : m_request_reiceived{ limit_request_reiceived } {
        m_server_engine = std::make_unique<protocol::engine::NetEngineServer<NetServerStub>>(*this, static_cast<unsigned short>(6060), limit);
    }

    void run() {
        m_server_engine->run();
    }

    void on_request_read(const protocol::Request &request) {
        m_request_collection.push_back(request);
        --m_request_reiceived;
        if (m_request_reiceived == 0) {
            m_server_engine->stop();
        }
    }

    std::vector<protocol::Request> get_request() { return m_request_collection; };
private:
    std::vector<protocol::Request> m_request_collection;
    std::unique_ptr<protocol::engine::NetEngineServer<NetServerStub>> m_server_engine;
    unsigned m_request_reiceived;
};


static void run_client() {
    try {
        asio::io_service io_service;

        tcp::resolver resolver{ io_service };
        std::error_code ec;
        auto endpoint_iterator = resolver.resolve({ "localhost", "6060" }, ec);
        protocol::NetClient c{ io_service, endpoint_iterator };
        c.connect([&](auto &&) { },
                  [&](std::error_code) { });
        io_service.run();
        c.close();
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

class WritingTest {
    using ptr_net_client = std::unique_ptr<protocol::NetClient>;
    using ptr_input = std::unique_ptr<protocol::io::NetIO<protocol::io::IoType::INPUT, WritingTest>>;
public:
    WritingTest()  : m_resolver{ m_io_service } {
        auto endpoint_iterator = m_resolver.resolve({ "localhost", "6060" });
        m_client = std::make_unique<protocol::NetClient>( m_io_service, endpoint_iterator );
    }
    void run(protocol::Request const &request, unsigned = 1) {
        m_client->connect([&](auto &&socket) {
            m_socket = std::move(socket);
            read_data();
            m_input = std::make_unique<protocol::io::NetIO<protocol::io::IoType::INPUT, WritingTest>>( m_socket.get(), *this );
            m_data = new char[request.size()];
            request.data(m_data);
            m_input->write(m_data, request.size());
        }, [](std::error_code e) { std::cerr << "error in connect : " << e.message() << std::endl; });
        m_io_service.run();
        //std::cout << "Service closed by distant host" << std::endl;
        m_client->close();
    }
    void read_data() {
        m_socket->async_read_some(asio::buffer(m_data_arr.data(), 25), [&, this] (const std::error_code &ec, std::size_t) {
            if (!ec) {
                std::cout << m_data_arr.data() << std::endl;
                read_data();
            } else {
                std::cerr << "Error in read client : " << ec.message() << std::endl;
            }
        });
    }

    void on_write_success() {
        std::cout << "Data wrote successfully" << std::endl;
    }
    void on_write_failed(const std::error_code& error_code) {
        std::cerr << error_code.message() << std::endl;
    }
private:
    asio::io_service m_io_service;
    tcp::resolver m_resolver;
    ptr_net_client m_client;
    std::array<char, 25> m_data_arr;
    std::unique_ptr<tcp::socket> m_socket;
    ptr_input m_input;
    char *m_data;
};

static void run_client_io(unsigned nbr_request = 1) {
    try {
        WritingTest w{};
        protocol::Request r{ 0x2A, 1, protocol::OpCode::LEAVE, 0 };
        w.run(r, nbr_request);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

static std::vector<protocol::Request> run_server(unsigned limit = 1, unsigned limit_request = 1) {
    NetServerStub s{ limit, limit_request };
    s.run();
    return s.get_request();
}

TEST(Net_engine, write_with_one_client) {
    std::thread t_server{ []() { run_server(); } };
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::thread t_client{ []() { run_client_io(); } };

    t_server.join();
    t_client.join();
}

TEST(Net_engine, write_with_lot_of_clients) {
    std::thread t_server{ []() {
        auto request_collection = run_server(20, 20);
        EXPECT_EQ(request_collection.size(), static_cast<unsigned>(20));
        for (auto it : request_collection) {
            EXPECT_EQ(it.header.magic, 0x2A);
            EXPECT_EQ(it.header.revision, 1);
            EXPECT_EQ(it.header.opCode, protocol::OpCode::LEAVE);
            EXPECT_EQ(it.header.bufferSize, static_cast<unsigned>(0));
            EXPECT_EQ(it.body.size(), static_cast<unsigned>(0));
        }
    }};
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::vector<std::unique_ptr<std::thread>> v_clients{};
    for (unsigned i = 0; i < 20; ++i) {
        auto t_client = std::make_unique<std::thread>([](){ run_client_io(); });
        v_clients.push_back(std::move(t_client));
    }
    for (auto &threadPtr : v_clients) {
        threadPtr->join();
    }
    t_server.join();
}

TEST(Net_engine, simple_net_engine_connection) {
    std::thread t_server{ []() { run_server(); }};
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::thread t_client{ []() { run_client(); } };

    t_server.join();
    t_client.join();
}

TEST(Net_engine, multiple_net_engine_connection) {
    std::thread t_server{ []() { run_server(20); }};
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::vector<std::unique_ptr<std::thread>> v_client{};
    for (unsigned i = 0; i < 20; ++i) {
        v_client.push_back(std::make_unique<std::thread>(std::thread{ [](){ run_client(); } }));
    }
    for (auto &it : v_client) {
        it->join();
    }
    t_server.join();
}*/
