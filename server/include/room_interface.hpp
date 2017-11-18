//
// Created by Clement on 11/14/2017.
//

#ifndef SPACE_BATTLE_ROOM_INTERFACE_HPP
#define SPACE_BATTLE_ROOM_INTERFACE_HPP

#include <memory>
#include "serialization/data_utils.hpp"
#include "general_room.hpp"
#include "game_room.hpp"
#include "room_information.hpp"

namespace space_battle {

/*! \brief Interface used to handle the room */
class RoomInterface {
    using game_room_ptr_t = std::unique_ptr<GameRoom>;
public:
    /*! \brief Construct the room interface with a limit of game the users can create
     * @param[in]   limit   The limit of game the users can create
    */
    RoomInterface(std::size_t limit = 20);

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

    /*! \brief Remove the client from the general room
     * @param[in]   clientId    The id of the client
     * @return the status code relative to the removal of a client
    */
    protocol::serialize::StatusCode remove_from_general(std::size_t clientId);

    /*! \brief Remove the client from the game room he belong
     * @param[in]   clientId    The id of the client
     * @return the status code of the request
    */
    protocol::serialize::StatusCode remove_from_game(std::size_t clientId);

    /*! \brief Create new game
     * @param[in]   name    The name of the game
     * @param[in]   nbrViewer   The number of viewer we can add
     * @param[in]   limitOfTime The limit of time
     * @return the id of the room
    */
    std::size_t create_game(const std::string &name, std::size_t nbrViewer, std::size_t limitOfTime);

    /*! \brief Get the limit of game
     * @return the limit of the game the client can make
    */
    std::size_t get_game_limit() const;

    /*! \brief check if the user is in the room
     * @param[in]   clientId    The id of the client
     * @return true if a user is in room, false otherwise
    */
    bool is_in_room(std::size_t clientId) const;

    /*! \brief return informations about the game information
     * @return vector of room information
    */
    std::vector<RoomInformation> get_game_info() const;

    /*! \brief return the number of connected clients
     * @return the number of client in the general room
    */
    std::size_t get_client_connected() const;
private:
    GeneralRoom m_general;
    std::vector<game_room_ptr_t> m_game_room;
    std::size_t m_game_limit;
};

} // namespace space_battle

#endif //SPACE_BATTLE_ROOM_INTERFACE_HPP
