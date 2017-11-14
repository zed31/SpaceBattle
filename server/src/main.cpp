//
// Created by 49844 on 14/11/2017.
//

#include <iostream>
#include "server_engine.hpp"

int main() {
    std::cout << "Hello world" << std::endl;
    space_battle::Engine spaceBattle{6060};
    try {
        spaceBattle.run();
    } catch (const std::exception &e) {
        std::cerr << "UNEXPECTED exception : " << e.what() << std::endl;
        spaceBattle.stop();
    }
    return 0;
}