//
// Created by 49844 on 29/10/2017.
//

#include <chrono>
#include <thread>
#include "Net.hpp"
#include "NetIO.hpp"
//#include "gtest/gtest.h"
#include <iostream>

using asio::ip::tcp;

void run_client() {
    try {
        std::cout << "RUNNING TEST" << std::endl;
        asio::io_service io_service;

        std::cout << "Creating resolver" << std::endl;
        tcp::resolver resolver{ io_service };
        std::cout << "resolver created" << std::endl;
        std::error_code ec;
        auto endpoint_iterator = resolver.resolve({ "localhost", "6060" }, ec);
        std::cout << ec.message() << std::endl;
        protocol::NetClient c{ io_service, endpoint_iterator };
        std::cout << "Connecting client" << std::endl;
        c.connect([](auto v) { std::cout << "connected" << std::endl; },
                  [](std::error_code code) { std::cerr << "error " << code.message() << std::endl; });
        io_service.run();
        std::cout << "Closing client" << std::endl;
    } catch (...) {
        std::cerr << "Exception: " << std::endl;
    }
}

void run_server() {
    try {
        std::cout << "RUNNING TEST" << std::endl;
        asio::io_service io_service;

        tcp::endpoint endp{tcp::v4(), 6060};
        protocol::NetServer n{ io_service, endp };
        n.accept([](auto v) { std::cout << "success" << std::endl; });
        io_service.run();
        std::cout << "Closing client" << std::endl;
    } catch (...) {
        std::cerr << "Exception: " << std::endl;
    }
}

int main(int argc, char** argv) {
    std::thread t2{ []() { run_server(); } };
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::thread t{ [](){ run_client(); } };
    t2.join();
    t.join();
    //::testing::InitGoogleTest(&argc, argv);
    return 0;
}
