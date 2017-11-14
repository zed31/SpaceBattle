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
  } else {

  }
}

} // namespace space_battle