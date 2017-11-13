//
// Created by 49844 on 12/11/2017.
//

#include <iostream>
#include "client.hpp"

int main() {
    try {
        protocol::client client{ "localhost", "6060" };
        std::unique_ptr<protocol::OutputConnection> out{};
        auto f_client_connect = [&](protocol::OutputConnection &outputConnection) {

            out = std::make_unique<protocol::OutputConnection>(std::move(outputConnection));

            auto f_write_success = [&](protocol::OutputConnection &) {
                out->read();
                //out->close();
                //client.stop();
            };

            auto f_write_failure = [&](std::error_code ec, protocol::OutputConnection &) {
                std::cerr << "Error : " << ec << std::endl;
                out->close();
                client.stop();
            };

            auto f_read_failure = [&](std::error_code, protocol::OutputConnection &out) {
                out.close();
                client.stop();
            };

            auto f_read_success = [&] (const protocol::serialize::Response &response, protocol::OutputConnection &) {
                std::cerr << "Receiving new response : " << response.size() << std::endl;
            };

            out->on_write_success(f_write_success);
            out->on_write_failure(f_write_failure);
            out->on_read_success(f_read_success);
            out->on_read_failure(f_read_failure);

            protocol::serialize::Request r{};
            out->write(r);
        };
        client.on_connect(f_client_connect);
        std::thread t(&protocol::client::run, &client);
        t.join();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
