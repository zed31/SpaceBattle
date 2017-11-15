//
// Created by Clement on 11/14/2017.
//

#ifndef SPACE_BATTLE_ROOM_INTERFACE_HPP
#define SPACE_BATTLE_ROOM_INTERFACE_HPP

#include "serialization/data_utils.hpp"
#include "general_room.hpp"

namespace space_battle {

struct RoomInformation {
    std::size_t roomId;
    protocol::serialize::StatusCode statusCode;
};

/*! \brief Interface used to handle the room */
class RoomInterface {
public:
    /*! \brief insert player on general room
     * @param[in]   clientId    the id of the client
     * return the room information
    */
    RoomInformation insert_on_general_room(std::size_t clientId);
private:
    GeneralRoom m_general;
};

} // namespace space_battle

#endif //SPACE_BATTLE_ROOM_INTERFACE_HPP
