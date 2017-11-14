//
// Created by Clement on 11/14/2017.
//

#ifndef SPACE_BATTLE_REQUEST_PROCESSER_HPP
#define SPACE_BATTLE_REQUEST_PROCESSER_HPP

#include "serialization/request.hpp"

namespace space_battle {

/*! \brief Process a specific request  */
class RequestProcesser {
public:
  /*! \brief Construct the request processer
   * @param[in] playerId    The id of the player
  */
  RequestProcesser(std::size_t playerId);
  /*! \brief process the request
   * @param[in] request The request the server needs to process
  */
  protocol::serialize::Response process(const protocol::serialize::Request &request) const;
private:
  std::size_t m_player_id;
};

} // namespace space_battle

#endif //SPACE_BATTLE_REQUEST_PROCESSER_HPP
