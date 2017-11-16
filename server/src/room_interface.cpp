//
// Created by Clement on 11/15/2017.
//

#include <algorithm>
#include <iostream>
#include "room_interface.hpp"

namespace space_battle {

RoomInformation RoomInterface::insert_on_general_room(std::size_t clientId) {
    auto statusCode = m_general.insert_client(clientId);
    auto roomId = m_general.get_room_id();
    return RoomInformation{ roomId, statusCode };
}

protocol::serialize::StatusCode RoomInterface::send_message(size_t roomId, std::size_t clientId, const std::string &message) {
    std::cout << "RoomInterface::send_message: Sending message to the room" << std::endl;
    if (roomId == m_general.get_room_id()) {
        std::cout << "RoomInterface::send_message: Room is general room" << std::endl;
        return m_general.send_message(clientId, message);
    }
    auto it = std::find_if(m_game_room.begin(), m_game_room.end(), [=](const auto &gamePtr) { return gamePtr->get_room_id() == roomId; });
    if (it == m_game_room.end()) {
        std::cout << "RoomInterface::send_message: No room found" << std::endl;
        //TODO : Specific code for the room
        return protocol::serialize::StatusCode::PERMISSION_DENIED;
    }
    std::cout << "RoomInterface::send_message: Game room found" << std::endl;
    return (*it)->send_message(clientId, message);
}

} // namespace space_battle
