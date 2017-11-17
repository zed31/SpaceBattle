//
// Created by Clement on 11/14/2017.
//

#include <iostream>
#include <room.hpp>
#include "serialization/response.hpp"
#include "request_processer.hpp"

namespace space_battle {

RequestProcesser::RequestProcesser(std::size_t playerId, RoomInterface &room) : m_player_id{ playerId }, m_room{ &room } {}

protocol::serialize::Response RequestProcesser::connect_user(const protocol::serialize::Request &request) {
    auto bodyContent = request.body.content();

    if (bodyContent.size() != 1) {
        std::cout << "RequestProcesser::connect_user: Format of the body not correct" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x01"});
    }

    auto roomInfo = m_room->insert_on_general_room(m_player_id);

    if (roomInfo.statusCode == protocol::serialize::StatusCode::CLIENT_ALREADY_IN_ROOM) {
        std::cout << "RequestProcesser::connect_user: Client already in a room" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::CLIENT_ALREADY_IN_ROOM, {});
    }

    m_nickname = *bodyContent.begin();
    std::cout << "RequestProcesser::connect_user: Setting new user nickname: " << m_nickname << std::endl;
    auto roomIdStr = std::to_string(roomInfo.roomId);
    //TODO: Insert chat messages here
    return protocol::make_response(protocol::serialize::StatusCode::CLIENT_CONNECTED, {roomIdStr});
}

protocol::serialize::Response RequestProcesser::process(const protocol::serialize::Request &request) {
    auto opCode = request.header.opCode;

    switch (opCode) {

        case protocol::serialize::OpCode::CLIENT_CONNECT: {
            std::cout << "RequestProcesser::process: Connect user with id: " << m_player_id << std::endl;
            return connect_user(request);
        };
        default: return protocol::make_response(protocol::serialize::StatusCode::PERMISSION_DENIED, {"tes", "vraiment", "trop", "con"});
    }
}

void RequestProcesser::remove_from_room() {
    std::cout << "RequestProcesser::remove_from_room: Removing client from general and gaming room" << std::endl;
    m_room->remove_from_general(m_player_id);
    m_room->remove_from_game(m_player_id);
}

} //namespace space_battle
