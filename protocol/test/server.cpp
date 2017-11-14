//
// Created by 49844 on 12/11/2017.
//

#include <thread>
#include "server.hpp"

int main() {
    protocol::server s{6060};

    auto on_read = [] (const protocol::serialize::Request &request, protocol::InputConnection &out) {
        std::cerr << "Read succeed, " << request.size() << std::endl;
        std::cerr << "magic : " << static_cast<int>(request.header.magic) << std::endl;
        std::cerr << "revision : " << request.header.revision << std::endl;
        std::cerr << "size of buffer : " << request.header.bufferSize << std::endl;
        for (auto it : request.body.content()) {
            std::cerr << "Body : " << it << std::endl;
        }
        auto response = protocol::make_response(protocol::serialize::StatusCode::OK, {"tes", "vraiment", "trop", "con"});
        out.write(response);
    };

    s.on_read_succeed(on_read);
    s.run();
    return 0;
}