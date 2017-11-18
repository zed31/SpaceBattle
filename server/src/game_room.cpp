//
// Created by Clement on 11/16/2017.
//

#include <algorithm>
#include <iostream>
#include "game_room.hpp"

namespace space_battle {

GameRoom::GameRoom(std::size_t roomId) : Room{ roomId } {}

GameRoom::GameRoom(std::size_t roomId, const std::string &name, std::size_t nbrViewer, std::size_t limitOfTime)
        : Room{ roomId }, m_name{ name }, m_viewer_limit{ nbrViewer }, m_time_limit{ limitOfTime } {}

protocol::serialize::StatusCode GameRoom::remove_client(std::size_t clientId) {
    auto it = std::find(m_player_ids.begin(), m_player_ids.end(), clientId);

    std::cout << "GameRoom::remove_client: searching " << clientId << " inside " << std::endl;
    if (it == m_player_ids.end()) {
        std::cout << "GameRoom::remove_client: " << clientId << " not found inside the player id collection, checking in the viewer now" << std::endl;
        auto itViewer = std::find(m_viewer_ids.begin(), m_viewer_ids.end(), clientId);

        if (itViewer == m_viewer_ids.end()) {
            std::cout << "GeneralRoom::remove_client: " << clientId << " not found on both containers" << std::endl;
            return protocol::serialize::StatusCode::CLIENT_NOT_FOUND;
        }

        std::cout << "GeneralRoom::remove_client: Removing " << clientId << " from the viewer queue" << std::endl;
        m_viewer_ids.erase(itViewer);
        return protocol::serialize::StatusCode::OK;
    }

    m_player_ids.erase(it);
    std::cout << "GeneralRoom::remove_client: Removing client " << clientId << " from the player container" << std::endl;
    return protocol::serialize::StatusCode::OK;
}

bool GameRoom::is_in_room(std::size_t clientId) const {
    std::cout << "GameRoom::is_in_room: Check if the client " << clientId << " is in the room" << std::endl;
    auto itPlayer = std::find(m_player_ids.begin(), m_player_ids.end(), clientId);

    if (itPlayer == m_player_ids.end()) {
        std::cout << "GameRoom::is_in_room: Client " << clientId << " doesn't exist in the player container" << std::endl;
        auto itViewer = std::find(m_viewer_ids.begin(), m_viewer_ids.end(), clientId);

        if (itViewer == m_viewer_ids.end()) {
            std::cout << "GameRoom::is_in_room: Client " << clientId << " Not found in viewer container" << std::endl;
            return false;
        }

        std::cout << "GameRoom::is_in_room: Client " << clientId << " found in the viewer container" << std::endl;
        return true;
    }

    std::cout << "GameRoom::is_in_room: Client " << clientId << " found in the player container" << std::endl;
    return true;
}

RoomInformation GameRoom::get_room_info() const {
    return RoomInformation{m_room_id, m_name, protocol::serialize::StatusCode::OK};
}

} // namespace space_battle
