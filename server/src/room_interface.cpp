//
// Created by Clement on 11/15/2017.
//

#include "room_interface.hpp"

namespace space_battle {

RoomInformation RoomInterface::insert_on_general_room(std::size_t clientId) {
    auto statusCode = m_general.insert_client(clientId);
    auto roomId = m_general.get_room_id();
    return RoomInformation{ roomId, statusCode };
}

} // namespace space_battle
