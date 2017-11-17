//
// Created by 49844 on 14/11/2017.
//

#include <iostream>
#include <room.hpp>
#include "input_connection_pool.hpp"

namespace space_battle {

Protocol::Protocol(InputConnectionPool &inputCollection,
                   std::size_t id,
                   output_ptr_t &&connection, RoomInterface &roomInterface)
    : m_id{id}, m_connection{std::move(connection)}, m_input_collection{&inputCollection},
      m_room_interface{ &roomInterface },
      m_request_processer{ std::make_unique<RequestProcesser>(m_id, *m_room_interface) } {}

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
  auto response = m_request_processer->process(request);
  input.write(response);
}

void Protocol::on_read_fail(const std::error_code, protocol::InputConnection &input) {
    std::cout << "Protocol::on_read_fail: Read failed, removing client from the room and close the connection" << std::endl;
    m_request_processer->remove_from_room();
    input.close();
}

} // namespace space_battle
