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
        std::cout << "new player added in a room : " << m_room_id << std::endl;
        return protocol::serialize::StatusCode::CLIENT_CONNECTED;

    } else {

        return protocol::serialize::StatusCode::CLIENT_ALREADY_IN_ROOM;

    }
}

protocol::serialize::StatusCode GeneralRoom::send_message(std::size_t clientId, const std::string &message) {
    std::cout << "GeneralRoom::send_message: Send message to the general room" << std::endl;
    auto it = std::find(m_client.begin(), m_client.end(), clientId);

    if (it == m_client.end()) {

        std::cout << "GeneralRoom::send_message: Permission denied" << std::endl;
        return protocol::serialize::StatusCode::PERMISSION_DENIED;

    }

    std::cout << "GeneralRoom::send_message: Message will be sent" << std::endl;
    return Room::send_message(clientId, message);
}

protocol::serialize::StatusCode GeneralRoom::remove_client(std::size_t clientId) {
    auto it = std::find(m_client.begin(), m_client.end(), clientId);

    std::cout << "GeneralRoom::remove_client: Removing client " << clientId << " from the general room" << std::endl;
    if (it == m_client.end()) {

        std::cout << "GeneralRoom::remove_client: Client " << clientId << " not found" << std::endl;
        return protocol::serialize::StatusCode::CLIENT_NOT_FOUND;

    }

    std::cout << "GeneralRoom::remove_client: Client " << clientId << " found, removing now" << std::endl;
    m_client.erase(it);
    return protocol::serialize::StatusCode::OK;
}

bool GeneralRoom::is_in_room(std::size_t clientId) const {
    std::cout << "GeneralRoom::is_in_room check if the user " << clientId << " is in the room" << std::endl;
    auto it = std::find(m_client.begin(), m_client.end(), clientId);

    if (it == m_client.end()) {

        std::cout << "GeneralRoom::is_in_room: User " << clientId << " is not in the room" << std::endl;
        return false;

    }
    std::cout << "GeneralRoom::is_in_room: User " << clientId << " is in the room" << std::endl;
    return true;
}

std::size_t GeneralRoom::get_nbr_client() const {
    return m_client.size();
}

} // namespace space_battle
