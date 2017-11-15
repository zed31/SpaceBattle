//
// Created by Clement on 11/14/2017.
//

#ifndef SPACE_BATTLE_ROOM_HPP
#define SPACE_BATTLE_ROOM_HPP

#include <vector>
#include "serialization/data_utils.hpp"

namespace space_battle {

/*! \brief The class that holds the room */
class Room {
public:
    /*! \brief Construct the room with a specific and unique id
     * @param[in]   room_id   The Id of the room
    */
    Room(std::size_t room_id);
    /*! \brief Send message to the chatroom
     * @param[in] clientId    The id of the client
     * @param[in] message     The message sent to the chatroom
     * return a status code to inform if the message is sent correctly
    */
    protocol::serialize::StatusCode send_message(std::size_t clientId, const std::string &message);
    /*! \brief return the id of the room */
    std::size_t get_room_id() const;
protected:
    std::size_t m_room_id;
};

} // namespace space_battle

#endif //SPACE_BATTLE_ROOM_HPP
