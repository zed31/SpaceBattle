//
// Created by Clement on 11/15/2017.
//

#include <algorithm>
#include <iostream>
#include "general_room.hpp"

namespace space_battle {

GeneralRoom::GeneralRoom(std::size_t room_id) : Room{ room_id } {}

protocol::serialize::StatusCode GeneralRoom::insert_client(std::size_t client) {
    auto it = std::find(m_client.begin(), m_client.end(), client);
    if (it == m_client.end()) {
        m_client.push_back(client);
        std::cout << "new player added in a room : " << client << std::endl;
        return protocol::serialize::StatusCode::CLIENT_CONNECTED;
    } else {
        return protocol::serialize::StatusCode::CLIENT_ALREADY_IN_ROOM;
    }
}

} // namespace space_battle
