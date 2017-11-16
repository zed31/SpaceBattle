//
// Created by Clement on 11/14/2017.
//

#ifndef SPACE_BATTLE_ROOM_INTERFACE_HPP
#define SPACE_BATTLE_ROOM_INTERFACE_HPP

#include <memory>
#include "serialization/data_utils.hpp"
#include "general_room.hpp"
#include "game_room.hpp"

namespace space_battle {

struct RoomInformation {
    std::size_t roomId;
    protocol::serialize::StatusCode statusCode;
};

/*! \brief Interface used to handle the room */
class RoomInterface {
    using game_room_ptr_t = std::unique_ptr<GameRoom>;
public:
    /*! \brief insert player on general room
     * @param[in]   clientId    the id of the client
     * return the room information
    */
    RoomInformation insert_on_general_room(std::size_t clientId);

    /*! \brief send message to a specific room
     * @param[in]   clientId    the id of the client
     * @param[in]   roomId      the id of the room
     * @param[in]   message     the message
     * @return the status code of the request
    */
    protocol::serialize::StatusCode send_message(size_t roomId, std::size_t clientId, const std::string &message);
private:
    GeneralRoom m_general;
    std::vector<game_room_ptr_t> m_game_room;
};

} // namespace space_battle

#endif //SPACE_BATTLE_ROOM_INTERFACE_HPP
