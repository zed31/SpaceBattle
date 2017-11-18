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
    auto playerIdStr = std::to_string(m_player_id);
    return protocol::make_response(protocol::serialize::StatusCode::CLIENT_CONNECTED, {playerIdStr, roomIdStr});
}

protocol::serialize::Response RequestProcesser::send_message(const protocol::serialize::Request &request) {
    auto bodyContent = request.body.content();

    if (bodyContent.size() != 2) {
        std::cout << "RequestProcesser::send_message: Player " << m_player_id << " send wrong number of arguments" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x01"});
    }

    try {

        std::cout << "RequestProcesser::send_message: sending message to the client" << std::endl;
        auto roomId = std::stoi(bodyContent[0]);
        auto message = bodyContent[1];
        auto statusCode = m_room->send_message(roomId, m_player_id, message);
        return protocol::make_response(statusCode, {});

    } catch (const std::out_of_range &) {

        std::cout << "RequestProcesser::send_message: Index ouf room out of range of an std::size_t congrat !" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x02"});

    } catch (const std::invalid_argument &) {

        std::cout << "RequestProcesser::send_message: Invalid argument" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x02"});

    }
}

protocol::serialize::Response RequestProcesser::leave() {
    std::cout << "User " << m_player_id << " want to leave, removing the client from the rooms" << std::endl;
    m_room->remove_from_game(m_player_id);
    m_room->remove_from_general(m_player_id);
    return protocol::make_response(protocol::serialize::StatusCode::LEAVE_SUCCESS, {});
}

protocol::serialize::Response RequestProcesser::number_game() {
    auto gameLimit = m_room->get_game_limit();
    auto str = std::to_string(gameLimit);
    return protocol::make_response(protocol::serialize::StatusCode::NUMBER_GAME_MAKABLE, {str});
}

protocol::serialize::Response RequestProcesser::create_game(const protocol::serialize::Request &request) {
    std::cout << "RequestProcesser::create_game: Get body content and check the parameters" << std::endl;
    auto bodyContent = request.body.content();

    if (m_room->is_in_room(m_player_id)) {
        std::cout << "RequestProcesser::create_game: User already in a room and cannot create a game" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::PERMISSION_DENIED, {});
    }

    auto limitOfGame = m_room->get_game_limit();
    if (!limitOfGame) {
        std::cout << "RequestProcesser::create_game: No more game can be made" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x06"});
    }

    if (bodyContent.size() != 4) {
        std::cout << "RequestProcesser::create_game: Invalid body size" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x01"});
    }

    auto gameName = bodyContent[0];
    std::size_t nbrViewer{};
    std::size_t limitOfTime{};

    try {
        nbrViewer = std::stoi(bodyContent[2]);
    } catch (const std::out_of_range &) {
        std::cout << "RequestProcesser::create_game: Invalid number of viewers" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x02"});
    } catch (const std::invalid_argument &) {
        std::cout << "RequestProcesser::create_game: Invalid number of viewers" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x02"});
    }

    try {
        limitOfTime = std::stoi(bodyContent[3]);
    } catch (const std::out_of_range &) {
        std::cout << "RequestProcesser::create_game: Invalid limit of time" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x05"});
    } catch (const std::invalid_argument &) {
        std::cout << "RequestProcesser::create_game: Invalid limit of time" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x05"});
    }

    auto hasViewer = bodyContent[1];
    if (hasViewer.size() != 1 || (hasViewer[0] != '1' && hasViewer[0] != '0')) {
        std::cout << "RequestProcesser::create_game: Invalid has viewer" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x04"});
    }

    auto idRoom = m_room->create_game(gameName, hasViewer[0] ? nbrViewer : 0, limitOfTime);
    auto strIdRoom = std::to_string(idRoom);

    std::cout << "RequestProcesser::create_game: All good, creating the game now" << std::endl;
    return protocol::make_response(protocol::serialize::StatusCode::GAME_CREATED, {strIdRoom});
}

protocol::serialize::Response RequestProcesser::get_game_info() {
    std::cout << "RequestProcesser::get_game_info: Get game information" << std::endl;
    auto gameInfos = m_room->get_game_info();
    std::vector<std::string> gameInfosSerialized;

    for (const auto &it : gameInfos) {
        gameInfosSerialized.push_back(std::to_string(it.roomId));
        gameInfosSerialized.push_back(it.name);
    }

    return protocol::make_response(protocol::serialize::StatusCode::GAME_INFO, gameInfosSerialized);
}

protocol::serialize::Response RequestProcesser::get_nbr_client_connected() {
    auto connectedClient = m_room->get_client_connected();
    auto connectedClientStr = std::to_string(connectedClient);
    return protocol::make_response(protocol::serialize::StatusCode::NUMBER_CLIENT_CONNECTED, {connectedClientStr});
}

protocol::serialize::Response RequestProcesser::join_as_player(const protocol::serialize::Request &request) {
    std::cout << "RequestProcesser::join_as_player: Joining as player, check if user is in room" << std::endl;

    auto isInRoom = m_room->is_in_room(m_player_id);
    if (isInRoom) {
        return protocol::make_response(protocol::serialize::StatusCode::PERMISSION_DENIED, {});
    }

    auto bodyContent = request.body.content();

    try {
        //TODO : Check if the game id is correct
    } catch (const std::out_of_range &) {
        //TODO : Out of range
    }

    //TODO : Register player
    return protocol::make_response(protocol::serialize::StatusCode::OK, {});
}

protocol::serialize::Response RequestProcesser::process(const protocol::serialize::Request &request) {
    auto opCode = request.header.opCode;

    switch (opCode) {

        case protocol::serialize::OpCode::CLIENT_CONNECT: {
            std::cout << "RequestProcesser::process: Connect user with id: " << m_player_id << std::endl;
            return connect_user(request);
        };
        case protocol::serialize::OpCode::SEND_MESSAGE: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " sent a message" << std::endl;
            return send_message(request);
        };
        case protocol::serialize::OpCode::LEAVE : {
            std::cout << "RequestProcesser::process: User " << m_player_id << " want to leave" << std::endl;
            return leave();
        };
        case protocol::serialize::OpCode::NUMBER_GAME_MAKABLE: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " want to know how many game he can create" << std::endl;
            return number_game();
        };
        case protocol::serialize::OpCode::CREATE_GAME: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " want to create game" << std::endl;
            return create_game(request);
        };
        case protocol::serialize::OpCode::GET_GAME_INFO: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " want to get game information" << std::endl;
            return get_game_info();
        };
        case protocol::serialize::OpCode::NBR_PEOPLE_LOGGED_IN: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " want to know how many people are connected" << std::endl;
            return get_nbr_client_connected();
        };
        case protocol::serialize::OpCode::JOIN_AS_PLAYER: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " want to join a room as player" << std::endl;
            return join_as_player(request);
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
