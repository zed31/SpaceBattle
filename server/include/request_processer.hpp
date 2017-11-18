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
    protocol::serialize::Response process(const protocol::serialize::Request &request);

    /*! \brief Remove the user from game and general room usually called when a read fail */
    void remove_from_room();
private:
    protocol::serialize::Response connect_user(const protocol::serialize::Request &request);
    protocol::serialize::Response send_message(const protocol::serialize::Request &request);
    protocol::serialize::Response leave();
    protocol::serialize::Response number_game();
    protocol::serialize::Response create_game(const protocol::serialize::Request &request);
    protocol::serialize::Response get_game_info();
    protocol::serialize::Response get_nbr_client_connected();
    protocol::serialize::Response join_as_player(const protocol::serialize::Request &request);
private:
    std::size_t m_player_id;
    std::size_t m_room_information;
    RoomInterface *m_room;
    std::string m_nickname;

};

} // namespace space_battle

#endif //SPACE_BATTLE_REQUEST_PROCESSER_HPP
