//
// Created by 49844 on 14/11/2017.
//

#include <iostream>
#include "server_engine.hpp"

namespace space_battle {

Engine::Engine(unsigned short port) : m_server{port} {}

void Engine::run() {
    auto on_accept = [&, this] (std::unique_ptr<protocol::InputConnection> &&connection) {
        m_input_collection.insert(std::move(connection));
    };
    m_server.on_accept(on_accept);
    m_server.run();
}

void Engine::stop() {
    m_server.stop();
}

} // namespace space_battle