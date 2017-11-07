//
// Created by 49844 on 29/10/2017.
//

#include <thread>
#include "Net.hpp"
#include "NetIO.hpp"
//#include "gtest/gtest.h"
#include <iostream>

int main(int argc, char** argv) {
    using asio::ip::tcp;
    std::cout << "RUNNING TEST" << std::endl;
    asio::io_service io_service;

    std::cout << "Creating resolver" << std::endl;
    tcp::resolver resolver{ io_service };
    try {

        std::cout << "resolver created" << std::endl;
        std::cout << "RUNNING TEST" << std::endl;
        std::cout << "RUNNING TEST" << std::endl;
        auto endpoint_iterator = resolver.resolve({ "localhost", "4242" });
        std::cout << "RUNNING TEST" << std::endl;
        protocol::NetClient c(io_service, endpoint_iterator);
        std::cout << "RUNNING TEST" << std::endl;
        std::thread t{ [&io_service]() { io_service.run(); } };
        std::cout << "ON APPVEYOR, it works" << std::endl;
        c.doConnect([](auto v) { std::cout << "connected" << std::endl; },
                    [](std::error_code code) { std::cerr << "error " << code.message() << std::endl; });
        std::cout << "ON APPVEYOR, it works" << std::endl;
        c.close();
        t.join();
    } catch (...) {
        std::cerr << "Exception: " << std::endl;
    }
    //::testing::InitGoogleTest(&argc, argv);
    return 0;
}
