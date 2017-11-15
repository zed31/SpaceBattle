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
    */
    protocol::serialize::StatusCode insert_client(std::size_t client);
private:
    std::vector<std::size_t> m_client;
};

} // namespace space_battle

#endif //SPACE_BATTLE_GENERALROOM_HPP
