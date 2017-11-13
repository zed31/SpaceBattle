//
// Created by 49844 on 12/11/2017.
//

#include <thread>
#include "server.hpp"

int main() {
    protocol::server s{6060};

    auto on_read = [] (const protocol::serialize::Request &request, protocol::InputConnection &out) {
        std::cerr << "Read succeed, " << request.size() << std::endl;
        out.write();
    };

    s.on_read_succeed(on_read);
    s.run();
    return 0;
}