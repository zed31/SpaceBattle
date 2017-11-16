//
// Created by Clement on 11/16/2017.
//

#ifndef SPACE_BATTLE_GAME_ROOM_HPP
#define SPACE_BATTLE_GAME_ROOM_HPP

#include <vector>
#include "room.hpp"
#include "non_copyable.hpp"

namespace space_battle {

/*! \brief Handling game and player / viewers inside */
class GameRoom : public Room, public NonCopyable {
public:
    GameRoom(std::size_t roomId);
private:
    std::size_t m_viewer_limit;
    std::size_t m_time_limit;
};

} // namespace space_battle

#endif //SPACE_BATTLE_GAME_ROOM_HPP
