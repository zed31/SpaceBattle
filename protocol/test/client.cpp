//
// Created by 49844 on 12/11/2017.
//

#include <iostream>
#include "Client.hpp"

int main() {
    try {
        protocol::Client client{ "localhost", "6060" };
        auto f_client_connect = [&]() {
            //auto output = std::move(outputConnection);
            //output->read();
            std::cout << "connection complete" << std::endl;
        };
        client.run(f_client_connect);
        while (1);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
