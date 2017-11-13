//
// Created by 49844 on 12/11/2017.
//

#include <iostream>
#include "client.hpp"

int main() {
    try {
        /*
         * Creating client with the domain and port
         * THIS IS NOT WHERE THE CLIENT MAKE A CONNECTION
        */
        protocol::client client{ "127.0.0.1", "6060" };

        /*
         * The callback called when a client connects itself to the server
         * this function is triggered when a connection happened
         * the given outputConnection parameter allows you to handle read/write/close on the socket !
        */
        auto on_connect = [&](protocol::OutputConnection &outputConnection) {

            /*
             * You must define function if you want to handle the "return" of write, this function is called
             * when what you want to wrote on the socket finished to write.
             * when this function is called, the API calls it with OutputConnection and Request as parameter
             * the request is the one you wrote !
            */
            auto on_write_success = [&](protocol::OutputConnection &out, const protocol::serialize::Request &) {
                out.read();
            };
            outputConnection.on_write_success(on_write_success);

            /*
             * You must define this function if you want to handle when a write fail (when there is a problem with the
             * network). You should probably close the socket (at least) with the close function from the
             * OutputConnection
            */
            auto on_write_failure = [&](std::error_code ec, protocol::OutputConnection &out, const protocol::serialize::Request &) {
                std::cerr << "Error : " << ec << std::endl;
                out.close();
                client.stop();
            };
            outputConnection.on_write_failure(on_write_failure);

            /*
             * You must define this function if you want to handle when a read fails (when there is a problem with the
             * network). You should probably close the socket (at least) with the close function from the
             * OutputConnection
            */
            auto on_read_failure = [&](std::error_code, protocol::OutputConnection &out) {
                out.close();
                client.stop();
            };
            outputConnection.on_read_failure(on_read_failure);

            /*
             * Define it when you want to handle the success of a read.
            */
            auto on_read_success = [&] (const protocol::serialize::Response &response, protocol::OutputConnection &out) {
                std::cerr << "Receiving new response : " << response.size() << std::endl;
                std::cerr << "magic : " << response.header.magic << std::endl;
                std::cerr << "revision : " << response.header.revision << std::endl;
                std::cerr << "size of buffer : " << response.header.bufferSize << std::endl;
                for (auto it : response.body.content()) {
                    std::cerr << "Body : " << it << std::endl;
                }
                out.close();
                client.stop();
            };
            outputConnection.on_read_success(on_read_success);

            //only for testing the app !
            protocol::serialize::Request r{};
            outputConnection.write(r);
        };
        /*
         * Used to bind the "on_connect" function within the client.
        */
        client.on_connect(on_connect);
        /*
         * This example use the client inside another thread, but you can define it in the main thread by using the
         * run function (after defining the on_connect method of course) :
         * ********************************
         * client.on_connect(on_connect);
         * client.run();
         * ********************************
        */
        std::thread t(&protocol::client::run, &client);
        t.join();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
