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
  /*! \brief Insert the client inside the viewers queue */
  protocol::serialize::StatusCode insert_viewer(std::size_t viewerId);

  /*! \brief Insert the client inside the player queue */
  protocol::serialize::StatusCode insert_player(std::size_t playerId);
private:
  std::vector<std::size_t> m_player_id;
  std::vector<std::size_t> m_viewer_id;
};

} // namespace space_battle

#endif //SPACE_BATTLE_ROOM_HPP
