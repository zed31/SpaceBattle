//
// Created by 49844 on 12/11/2017.
//

#include <thread>
#include "Server.hpp"

int main() {
    protocol::Server s{6060};
    s.run();
    return 0;
}