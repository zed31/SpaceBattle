//
// Created by Clement on 11/14/2017.
//

#include <algorithm>
#include "room.hpp"

namespace space_battle {

protocol::serialize::StatusCode Room::insert_viewer(std::size_t viewerId) {
  auto it = std::find(m_viewer_id.begin(), m_viewer_id.end(), viewerId);
  if (it == m_viewer_id.end()) {
    m_viewer_id.push_back(viewerId);
    return protocol::serialize::StatusCode::CLIENT_CONNECTED;
  } else {
    return protocol::serialize::StatusCode::USER_ALREADY_IN_ROOM;
  }
}

protocol::serialize::StatusCode Room::insert_player(std::size_t viewerId) {
  auto it = std::find(m_player_id.begin(), m_player_id.end(), viewerId);
  if (it == m_player_id.end()) {
    m_player_id.push_back(viewerId);
    return protocol::serialize::StatusCode::CLIENT_CONNECTED;
  } else {
    return protocol::serialize::StatusCode::USER_ALREADY_IN_ROOM;
  }
}

} // namespace space_battle