//
// Created by Clement on 11/16/2017.
//

#ifndef SPACE_BATTLE_GAME_ROOM_HPP
#define SPACE_BATTLE_GAME_ROOM_HPP

#include <vector>
#include "room.hpp"
#include "non_copyable.hpp"
#include "room_information.hpp"

namespace space_battle {

/*! \brief Handling game and player / viewers inside */
class GameRoom : public Room, public NonCopyable {
public:
    /*! \brief Status of the game */
    enum GameStatus { NEW, WAITING_FOR_PLAYER, WAITING_FOR_LAUNCHING, IN_PROGRESS };

    GameRoom(std::size_t roomId);

    /*! \brief Create a room with the parameter
     * @param[in]   roomId  The id of the room
     * @param[in]   name    The name of the room
     * @param[in]   limitOfTime the limit of time
     * @param[in]   nbrViewer   The number of viewers
    */
    GameRoom(std::size_t roomId, const std::string &name, std::size_t nbrViewer, std::size_t limitOfTime);

    /*! \brief remove a client from the collection of viewer or players
     * @param[in]   clientId    The id of the client
     * @return the status relative to the request
    */
    protocol::serialize::StatusCode remove_client(std::size_t clientId);

    /*! \brief Check if the specified client is in the room or not
     * @param[in]   clientId    the id of the client
     * @return true if the client is in the room, false otherwise
    */
    bool is_in_room(std::size_t clientId) const;

    /*! \brief return room information */
    RoomInformation get_room_info() const;
private:
    std::string m_name;
    std::size_t m_viewer_limit;
    std::size_t m_time_limit;
    GameStatus m_room_status;
    std::vector<std::size_t> m_viewer_ids;
    std::vector<std::size_t> m_player_ids;
};

} // namespace space_battle

#endif //SPACE_BATTLE_GAME_ROOM_HPP
