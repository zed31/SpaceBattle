//
// Created by Clement on 11/15/2017.
//

#include <algorithm>
#include <iostream>
#include "room_interface.hpp"

namespace space_battle {

RoomInterface::RoomInterface(size_t limit) : m_game_limit{ limit } {}

RoomInformation RoomInterface::insert_on_general_room(std::size_t clientId) {
    auto statusCode = m_general.insert_client(clientId);
    auto roomId = m_general.get_room_id();
    return RoomInformation{ roomId, "general", statusCode };
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
        return protocol::serialize::StatusCode::ROOM_NOT_FOUND;
    }

    std::cout << "RoomInterface::send_message: Game room found" << std::endl;
    return (*it)->send_message(clientId, message);
}

protocol::serialize::StatusCode RoomInterface::remove_from_general(std::size_t clientId) {
    std::cout << "RoomInterface::remove_from_general: Removing " << clientId << " from the general room" << std::endl;
    return m_general.remove_client(clientId);
}

protocol::serialize::StatusCode RoomInterface::remove_from_game(std::size_t clientId) {
    std::cout << "RoomInterface::remove_from_game: Removing " << clientId << " from the room" << std::endl;

    auto it = std::find_if(m_game_room.begin(), m_game_room.end(), [clientId](const auto &gameRoom) { return gameRoom->is_in_room(clientId); });
    if (it == m_game_room.end()) {
        std::cout << "RoomInterface::remove_from_game: Client " << clientId << " not found in any game room" << std::endl;
        return protocol::serialize::StatusCode::CLIENT_NOT_FOUND;
    }

    return (*it)->remove_client(clientId);
}

std::size_t RoomInterface::get_game_limit() const {
    return m_game_limit;
}

bool RoomInterface::is_in_room(std::size_t clientId) const {
    auto it = std::find_if(m_game_room.begin(), m_game_room.end(), [clientId](const auto &gamePtr) { return gamePtr->is_in_room(clientId); });
    return it != m_game_room.end();
}

std::size_t RoomInterface::create_game(const std::string &name, std::size_t nbrViewer, std::size_t limitOfTime) {
    auto idRoom = m_game_room.size() + 1;
    auto gameRoom = std::make_unique<GameRoom>(idRoom, name, nbrViewer, limitOfTime);
    m_game_room.push_back(std::move(gameRoom));
    return idRoom;
}

std::vector<RoomInformation> RoomInterface::get_game_info() const {
    std::cout << "RoomInterface::get_game_info: Get game information" << std::endl;
    std::vector<RoomInformation> gameInfo{};

    for (const auto &gamePtr : m_game_room) {
        gameInfo.push_back(gamePtr->get_room_info());
    }

    return gameInfo;
}

std::size_t RoomInterface::get_client_connected() const {
    m_general.get_nbr_client();
}

} // namespace space_battle
