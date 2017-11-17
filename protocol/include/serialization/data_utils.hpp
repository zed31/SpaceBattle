//
// Created by 49844 on 12/11/2017.
//

#ifndef SPACE_BATTLE_DATAUTILS_HPP
#define SPACE_BATTLE_DATAUTILS_HPP

#include <string>

namespace protocol {

namespace serialize {

/*! \brief This enumeration contain all the response possible the server can sent */
enum class StatusCode : unsigned short {
    OK = 0xB000,
    PERMISSION_DENIED = 0xB001,
    CLIENT_CONNECTED = 0xB003,
    CONNECTION_REFUSED = 0xB002,
    FORMAT_ERROR = 0xB004,
    CLIENT_DISCONNECT = 0xB005,
    EMPTY_BODY_FAILURE = 0xB006,
    INVALID_BODY_SIZE = 0xB007,
    INVALID_MAGIC_CODE = 0xB008,
    INVALID_REVISION_NUMBER = 0xB009,
    INVALID_OPCODE = 0xB010,
    NUMBER_OF_CLIENT_MAX_REACHED = 0xB011,
    NUMBER_GAME_MAKABLE = 0xB012,
    LIST_GAME = 0xB113,
    NICKNAME_NOT_SET = 0xB114,
    CLIENT_NOT_FOUND = 0xB115,
    GAME_CREATED = 0xB101,
    TOO_MANY_CLIENTS = 0xB100,
    GAME_ID_DOESNT_EXIST = 0xB103,
    CLIENT_JOIN_GAME = 0xB102,
    GAME_INFO = 0xB105,
    NICKNAME_PLAYER_LOGGED_IN = 0xB106,
    PLAYER_NUMBER = 0xB107,
    NUMBER_OF_CLIENT_CONNECTABLE = 0xB108,
    GAME_WILL_DESTROY = 0xB110,
    CLIENT_ALREADY_IN_ROOM = 0xB115,
    CLIENT_ARRIVED_IN_GAME = 0xB111,
    GAME_CHANGE_STATUS = 0xB112,
    USER_ALREADY_IN_ROOM = 0xB113,
    WAITING_FOR_SECOND_PLAYER = 0xB120,
    READY_TO_PLAY = 0xB121,
    MISS_SHIP = 0xB122,
    HIT_SHIP = 0xB123,
    SUNK_SHIP = 0xB212,
    PLAYER_WIN = 0xB213
};

/*! \brief This enumeration contain all the op code possible to send to the server */
enum class OpCode : unsigned short {
    SEND_MESSAGE = 0xA100,
    CLIENT_DISCONNECT = 0xA101,
    NUMBER_GAME_MAKABLE = 0xA102,
    CLIENT_CONNECT = 0xA103,
    NUMBER_GAME_CREATED = 0xA110,
    CREATE_GAME = 0xA111,
    JOIN_AS_PLAYER = 0xA112,
    JOIN_AS_VIEWER = 0xA113,
    GET_GAME_INFO = 0xA114,
    PEOPLE_LOGGED_IN = 0xA115,
    NBR_PEOPLE_LOGGED_IN = 0xA116,
    READY_TO_PLAY = 0xA120,
    TARGET = 0xA121,
    LEAVE = 0xA122,
    REPLAY = 0xA123,
    REFUSE_REPLAY = 0xA124,
    PLACE_SHIP = 0xA125
};

struct HeaderUtility {
    static constexpr short default_rev_ = 1;
    static constexpr char default_magic_ = 0x2A;
    static constexpr unsigned default_buffer_size_ = 0;
    template<typename Nbr>
    static std::string convert(Nbr value) {
        auto maxBoundary = std::to_string(std::numeric_limits<Nbr>::max());
        auto valueStr = std::to_string(value);
        if (valueStr.size() < maxBoundary.size()) {
            auto paddedStr = std::string(maxBoundary.size() - valueStr.size(), '0') + valueStr;
            return paddedStr;
        }
        return valueStr;
    };
};

} // namespace serialize

} // namespace protocol

#endif //SPACE_BATTLE_DATAUTILS_HPP
