//
// Created by Clement on 11/14/2017.
//

#ifndef SPACE_BATTLE_REQUEST_PROCESSER_HPP
#define SPACE_BATTLE_REQUEST_PROCESSER_HPP

#include <string>
#include "serialization/request.hpp"
#include "room_interface.hpp"

namespace space_battle {

/*! \brief Process a specific request  */
class RequestProcesser {
public:
    /*! \brief Construct the request processer
     * @param[in] playerId    The id of the player
     * @param[in] room        The interface of the room
    */
    RequestProcesser(std::size_t playerId, RoomInterface &room);
    /*! \brief process the request
     * @param[in] request The request the server needs to process
    */
    protocol::serialize::Response process(const protocol::serialize::Request &request) const;
private:
    std::size_t m_player_id;
    RoomInterface *m_room;
};

} // namespace space_battle

#endif //SPACE_BATTLE_REQUEST_PROCESSER_HPP
