//
// Created by Clement on 11/16/2017.
//

#include <algorithm>
#include <iostream>
#include "game_room.hpp"

namespace space_battle {

GameRoom::GameRoom(std::size_t roomId) : Room{ roomId } {}

GameRoom::GameRoom(std::size_t roomId, const std::string &name, std::size_t nbrViewer, std::size_t limitOfTime)
        : Room{ roomId }, m_name{ name }, m_viewer_limit{ nbrViewer }, m_time_limit{ limitOfTime }, m_room_status{GameStatus::NEW} {}

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

GameRoom::GameInformation GameRoom::get_game_info() const {
    return GameRoom::GameInformation{m_name, m_viewer_limit, m_player_ids.size(), m_time_limit, m_room_status};
}

protocol::serialize::StatusCode GameRoom::insert_player(std::size_t clientId) {
    std::cout << "GameRoom::insert_player: client " << clientId << " is about to be inserted in the game as player" << std::endl;
    auto nbrPlayer = m_player_ids.size();

    if (nbrPlayer == 2) {
        std::cout << "GameRoom::insert_player: No more place available for a new player, not inserting it" << std::endl;
        return protocol::serialize::StatusCode::TOO_MANY_CLIENTS;
    }

    m_player_ids.push_back(clientId);
    std::cout << "GameRoom::insert_player: Insertion success" << std::endl;
    return protocol::serialize::StatusCode::CLIENT_JOIN_GAME;
}

protocol::serialize::StatusCode GameRoom::insert_viewer(std::size_t clientId) {
    std::cout << "GameRoom::insert_viewer: Inserting " << clientId << " as a viewer to the game" << std::endl;

    if (m_viewer_limit == 0) {
        std::cout << "GameRoom::insert_viewer: No more place available, exiting now" << std::endl;
        return protocol::serialize::StatusCode::TOO_MANY_CLIENTS;
    }

    m_viewer_ids.push_back(clientId);
    std::cout << "GameRoom::insert_viewer: Inserting viewer " << clientId << " success" << std::endl;
    return protocol::serialize::StatusCode::CLIENT_JOIN_GAME;
}

} // namespace space_battle
