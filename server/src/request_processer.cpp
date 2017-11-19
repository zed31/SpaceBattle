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
    return protocol::make_response(protocol::serialize::StatusCode::LEAVE_SUCCESS, {});
}

protocol::serialize::Response RequestProcesser::create_game(const protocol::serialize::Request &request) {
    std::cout << "RequestProcesser::create_game: Get body content and check the parameters" << std::endl;
    auto bodyContent = request.body.content();

    if (m_room->is_in_room(m_player_id)) {
        std::cout << "RequestProcesser::create_game: User already in a room and cannot create a game" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::PERMISSION_DENIED, {});
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
        if (limitOfTime != 0 && limitOfTime != 30 && limitOfTime != 90) {

            std::cout << "RequestProcesser::create_game: Invalid limit of time" << std::endl;
            return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x04"});

        }

    } catch (const std::out_of_range &) {
        std::cout << "RequestProcesser::create_game: Invalid limit of time" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x04"});
    } catch (const std::invalid_argument &) {
        std::cout << "RequestProcesser::create_game: Invalid limit of time" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x04"});
    }

    auto hasViewer = bodyContent[1];
    if (hasViewer.size() != 1 || (hasViewer[0] != '1' && hasViewer[0] != '0')) {
        std::cout << "RequestProcesser::create_game: Invalid has viewer" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x03"});
    }

    auto idRoom = m_room->create_game(m_player_id, gameName, hasViewer[0] ? nbrViewer : 0, limitOfTime);
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

    return protocol::make_response(protocol::serialize::StatusCode::LIST_GAME, gameInfosSerialized);
}

protocol::serialize::Response RequestProcesser::get_nbr_client_connected() {
    auto connectedClient = m_room->get_client_connected();
    auto connectedClientStr = std::to_string(connectedClient);
    return protocol::make_response(protocol::serialize::StatusCode::NUMBER_CLIENT_CONNECTED, {connectedClientStr});
}

protocol::serialize::Response RequestProcesser::join_game_room(const protocol::serialize::Request &request, bool playerInsert) {
    std::cout << "RequestProcesser::join_game_room: Joining as player, check if user is in room" << std::endl;

    auto isInRoom = m_room->is_in_room(m_player_id);
    if (isInRoom) {
        std::cout << "RequestProcesser::join_game_room: Player already in a game room" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::PERMISSION_DENIED, {});
    }

    auto bodyContent = request.body.content();
    auto size = bodyContent.size();
    if (size != 1) {
        std::cout << "RequestProcesser::join_game_room: Incorrect body size" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x01"});
    }

    try {
        std::cout << "RequestProcesser::join_game_room: Checking the game id" << std::endl;
        auto gameId = std::stoi(bodyContent[0]);
        auto gameRoomIt = m_room->get_game(gameId);

        if (gameRoomIt == m_room->end()) {
            std::cout << "RequestProcesser::join_game_room: room " << gameId << " not found" << std::endl;
            return protocol::make_response(protocol::serialize::StatusCode::ROOM_NOT_FOUND, {});
        }

        protocol::serialize::StatusCode statusCode{};

        if (playerInsert) {
            statusCode = (*gameRoomIt)->insert_player(m_player_id);
        } else {
            statusCode = (*gameRoomIt)->insert_viewer(m_player_id);
        }

        if (statusCode == protocol::serialize::StatusCode::CLIENT_JOIN_GAME) {
            std::cout << "RequestProcesser::join_game_room: client " << m_player_id << " inserted inside the game " << gameId << std::endl;
            auto gameInformation = (*gameRoomIt)->get_game_info();
            auto gameName = gameInformation.name;
            auto viewerNbr = std::to_string(gameInformation.viewerLimit);
            auto timeLimit = std::to_string(gameInformation.timeLimit);
            auto playerNbr = std::to_string(gameInformation.nbrPlayer);
            auto gameStatus = std::to_string(static_cast<unsigned int>(gameInformation.status));
            return protocol::make_response(protocol::serialize::StatusCode::CLIENT_JOIN_GAME, {gameName, viewerNbr, playerNbr, timeLimit, gameStatus});
        }

        return protocol::make_response(protocol::serialize::StatusCode::TOO_MANY_CLIENTS, {});
    } catch (const std::out_of_range &) {
        std::cout << "RequestProcesser::join_game_room: GameID too big" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x01"});
    } catch (const std::invalid_argument &) {
        std::cout << "RequestProcesser::join_game_room: GameID invalid" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x01"});
    }
}

protocol::serialize::Response RequestProcesser::get_player_nbr(const protocol::serialize::Request &request) {
    std::cout << "RequestProcesser::get_player_nbr: Check if the body is valid" << std::endl;

    auto bodyContent = request.body.content();

    if (bodyContent.size() != 1) {
        std::cout << "RequestProcesser::get_player_nbr: Invalid body" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x01"});
    }

    std::cout << "RequestProcesser::get_player_nbr: Body valid, check validity of the game id" << std::endl;
    try {

        auto gameId = std::stoi(bodyContent[0]);
        std::cout << "RequestProcesser::get_player_nbr: Get game " << gameId << std::endl;
        auto gameRoomIt = m_room->get_game(gameId);
        if (gameRoomIt == m_room->end()) {

            std::cout << "RequestProcesser::get_player_nbr: Game room not found" << std::endl;
            return protocol::make_response(protocol::serialize::StatusCode::ROOM_NOT_FOUND, {});

        }

        auto nbrPlayers = std::to_string((*gameRoomIt)->get_game_info().nbrPlayer);
        return protocol::make_response(protocol::serialize::StatusCode::PLAYER_NUMBER, {nbrPlayers});

    } catch (const std::out_of_range &) {

        std::cout << "RequestProcesser::get_player_nbr: game id is out of range" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x01"});

    } catch (const std::invalid_argument &) {

        std::cout << "RequestProcesser::get_player_nbr: game id is invalid" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x01"});

    }
}

protocol::serialize::Response RequestProcesser::get_game_created() {
    std::cout << "RequestProcesser::get_game_created get the number of game created" << std::endl;

    auto nbrGame = std::to_string(m_room->nbr_game_created());
    return protocol::make_response(protocol::serialize::StatusCode::NUMBER_OF_GAME, {nbrGame});
}

protocol::serialize::Response RequestProcesser::get_game_status(const protocol::serialize::Request &request) {
    std::cout << "RequestProcesser::get_game_status: Get the content of the body" << std::endl;

    auto bodyContent = request.body.content();
    if (bodyContent.size() != 1) {

        std::cout << "RequestProcesser::get_game_status: Incorrect size of the body" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x01"});

    }

    try {

        std::cout << "RequestProcesser::get_game_status: Try to convert the bodyContent[0]: " << bodyContent[0] << " into a number" << std::endl;
        auto gameId = std::stoi(bodyContent[0]);
        auto gameIt = m_room->get_game(gameId);

        if (gameIt == m_room->end()) {

            std::cout << "RequestProcesser::get_game_status: Room not found" << std::endl;
            return protocol::make_response(protocol::serialize::StatusCode::ROOM_NOT_FOUND, {});

        }

        auto status = std::to_string(static_cast<unsigned>((*gameIt)->get_game_info().status));
        return protocol::make_response(protocol::serialize::StatusCode::GAME_STATUS, {status});

    } catch (const std::out_of_range &) {

        std::cout << "RequestProcesser::get_game_status: Conversion failed, number out of range" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x01"});

    } catch (const std::invalid_argument &) {

        std::cout << "RequestProcesser::get_game_status: Conversion failed, invalid argument" << std::endl;
        return protocol::make_response(protocol::serialize::StatusCode::FORMAT_ERROR, {"\x01"});

    }
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
            std::cout << "RequestProcesser::process: User " << m_player_id << " wants to leave" << std::endl;
            return leave();
        };
        case protocol::serialize::OpCode::CREATE_GAME: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " wants to create game" << std::endl;
            return create_game(request);
        };
        case protocol::serialize::OpCode::GET_GAME_INFO: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " wants to get game information" << std::endl;
            return get_game_info();
        };
        case protocol::serialize::OpCode::NBR_PEOPLE_LOGGED_IN: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " wants to know how many people are connected" << std::endl;
            return get_nbr_client_connected();
        };
        case protocol::serialize::OpCode::JOIN_AS_PLAYER: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " wants to join a room as player" << std::endl;
            return join_game_room(request);
        };
        case protocol::serialize::OpCode::JOIN_AS_VIEWER: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " wants to join a room as viewer" << std::endl;
            return join_game_room(request, false);
        };
        case protocol::serialize::OpCode::PLAYER_LOGGED_IN: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " wants to know number of player in a game" << std::endl;
            return get_player_nbr(request);
        };
        case protocol::serialize::OpCode::GET_NBR_GAME_CREATED: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " wants to know number of game created" << std::endl;
            return get_game_created();
        };
        case protocol::serialize::OpCode::GET_GAME_STATUS: {
            std::cout << "RequestProcesser::process: User " << m_player_id << " information about status of a game" << std::endl;
            return get_game_status(request);
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
