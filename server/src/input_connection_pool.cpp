//
// Created by 49844 on 14/11/2017.
//

#include <algorithm>
#include <iostream>
#include "input_connection_pool.hpp"

namespace space_battle {

InputConnectionPool::proto_id_t InputConnectionPool::insert(output_ptr_t &&connection) {
    std::size_t id = m_input_collection.size() + 1;
    auto protocol = std::make_unique<Protocol>(*this, id, std::move(connection), m_room_interface);
    protocol->run();
    m_input_collection.push_back({id, std::move(protocol)});
    return id;
}

void InputConnectionPool::remove(proto_id_t id) {
    m_input_collection.erase(std::remove_if(m_input_collection.begin(), m_input_collection.end(), [&](auto const &elem) {
        return elem.id == id;
    }), m_input_collection.end());
}

} // namespace space_battle