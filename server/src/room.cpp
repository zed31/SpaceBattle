//
// Created by Clement on 11/14/2017.
//

#include <algorithm>
#include <iostream>
#include "room.hpp"

namespace space_battle {

Room::Room(std::size_t room_id) : m_room_id{ room_id } {}

std::size_t Room::get_room_id() const {
    return m_room_id;
}

protocol::serialize::StatusCode Room::send_message(std::size_t clientId, const std::string &message) {
    std::cout << "Receive new message from : " << clientId << ": " << message << std::endl;
    return protocol::serialize::StatusCode::OK;
}

} // namespace space_battle