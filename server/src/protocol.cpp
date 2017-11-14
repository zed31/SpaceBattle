//
// Created by 49844 on 14/11/2017.
//

#include <iostream>
#include "input_connection_pool.hpp"
#include "protocol.hpp"

namespace space_battle {

Protocol::Protocol(InputConnectionPool &inputCollection,
                   std::size_t id,
                   output_ptr_t &&connection)
    : m_id{id}, m_connection{std::move(connection)}, m_input_collection{&inputCollection} {}

void Protocol::run() {
    auto on_read_succeed = [this](const auto &request, auto &input) {
        this->on_read_success(request, input);
    };

    auto on_read_failure = [this](auto error, auto &input) {
        this->on_read_fail(error, input);
    };

    auto on_close = [this]() {
        this->m_input_collection->remove(m_id);
    };

    m_connection->on_read_failed(on_read_failure);
    m_connection->on_close(on_close);
    m_connection->on_read_succeed(on_read_succeed);
    m_connection->read();
}

void Protocol::on_read_success(const protocol::serialize::Request &request, protocol::InputConnection &input) {
    std::cerr << "Read succeed, " << request.size() << std::endl;
    std::cerr << "magic : " << static_cast<int>(request.header.magic) << std::endl;
    std::cerr << "revision : " << request.header.revision << std::endl;
    std::cerr << "size of buffer : " << request.header.bufferSize << std::endl;
    for (auto it : request.body.content()) {
        std::cerr << "Body : " << it << std::endl;
    }
    auto response = protocol::make_response(protocol::serialize::StatusCode::OK, {"tes", "vraiment", "trop", "con"});
    input.write(response);
}

void Protocol::on_read_fail(const std::error_code error, protocol::InputConnection &input) {
    std::cerr << "on_read_fail: Read failed: " << error << std::endl;
    input.close();
}

} // namespace space_battle
