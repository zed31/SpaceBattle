//
// Created by Clement on 11/17/2017.
//

//
// Created by 49844 on 12/11/2017.
//

#include <iostream>
#include "client.hpp"

int main() {
    try {
        protocol::client client{ "127.0.0.1", "6060" };

        auto on_connect = [&](protocol::OutputConnection &outputConnection) {

            auto on_write_success = [&](protocol::OutputConnection &out, const protocol::serialize::Request &) {
                std::cout << "on_write_success: Writing success, processing read now" << std::endl;
                out.read();
            };
            outputConnection.on_write_success(on_write_success);

            auto on_write_failure = [&](std::error_code ec, protocol::OutputConnection &out, const protocol::serialize::Request &) {
                std::cerr << "on_write_failure: " << ec << std::endl;
                out.close();
                client.stop();
            };
            outputConnection.on_write_failure(on_write_failure);

            auto on_read_failure = [&](std::error_code ec, protocol::OutputConnection &out) {
                std::cerr << "on_read_failure: " << ec.message() << std::endl;
                out.close();
            };
            outputConnection.on_read_failure(on_read_failure);

            auto on_read_success = [&] (const protocol::serialize::Response &response, protocol::OutputConnection &out) {
                std::cerr << "Receiving new response : " << response.size() << std::endl;
                std::cerr << "magic : " << response.header.magic << std::endl;
                std::cerr << "revision : " << response.header.revision << std::endl;
                std::cerr << "size of buffer : " << response.header.bufferSize << std::endl;
                for (auto it : response.body.content()) {
                    std::cerr << "Body : " << it << std::endl;
                }
                out.close();
            };
            outputConnection.on_read_success(on_read_success);

            auto on_close = [&] (protocol::OutputConnection &) {
                std::cerr << "on_close: Stoping the server" << std::endl;
                client.stop();
            };
            outputConnection.on_close(on_close);

            //only for testing the app !
            auto request = protocol::make_request(protocol::serialize::OpCode::CLIENT_CONNECT, {"Gregorie"});
            outputConnection.write(request);
        };

        auto on_connection_fails = [&]() {
            std::cerr << "on_connection_fails: Stopping the connection" << std::endl;
            client.stop();
        };

        client.on_connect(on_connect);

        client.on_connection_failed(on_connection_fails);
        std::thread t(&protocol::client::run, &client);
        t.join();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
