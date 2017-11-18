//
// Created by Clement on 11/18/2017.
//

#ifndef SPACE_BATTLE_ROOM_INFORMATION_HPP
#define SPACE_BATTLE_ROOM_INFORMATION_HPP

#include "serialization/data_utils.hpp"

namespace space_battle {

struct RoomInformation {
    std::size_t roomId;
    std::string name;
    protocol::serialize::StatusCode statusCode;
};

} //namespace space_battle

#endif //SPACE_BATTLE_ROOM_INFORMATION_HPP
