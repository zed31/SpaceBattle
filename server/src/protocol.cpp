//
// Created by 49844 on 14/11/2017.
//

#include <iostream>
#include <room.hpp>
#include "input_connection_pool.hpp"
#include "request_processer.hpp"

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
  RequestProcesser p{ m_id, m_room_interface };
  auto response = p.process(request);
  input.write(response);
}

void Protocol::on_read_fail(const std::error_code error, protocol::InputConnection &input) {
  input.close();
}

} // namespace space_battle
