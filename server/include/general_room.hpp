//
// Created by Clement on 11/15/2017.
//

#ifndef SPACE_BATTLE_GENERALROOM_HPP
#define SPACE_BATTLE_GENERALROOM_HPP

#include <vector>
#include "room.hpp"

namespace space_battle {

class GeneralRoom : public Room {
public:
    /*! \brief construct a room with a specific id
     * @param[in]   room_id the id of the room
    */
    GeneralRoom(std::size_t room_id = 0);
    /*! \brief insert a client to the room, it returns a status code depending how the insertion goes on
     * @param[in] client  The id of the client
     * @return the status code of the request
    */
    protocol::serialize::StatusCode insert_client(std::size_t client);

    /*! \brief Send message to the chatroom but first, check if the user is in the room
     * @param[in] clientId    The id of the client
     * @param[in] message     The message sent to the chatroom
     * @return a status code to inform if the message is sent correctly
    */
    protocol::serialize::StatusCode send_message(std::size_t clientId, const std::string &message) override;

    /*! \brief Remove a client from the general room
     * @param[in]   clientId    the id of the client
     * @return the status code of the request
    */
    protocol::serialize::StatusCode remove_client(std::size_t clientId);
private:
    std::vector<std::size_t> m_client;
};

} // namespace space_battle

#endif //SPACE_BATTLE_GENERALROOM_HPP
