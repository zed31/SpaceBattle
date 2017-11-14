//
// Created by Clement on 11/14/2017.
//

#include <iostream>
#include "serialization/response.hpp"
#include "request_processer.hpp"

namespace space_battle {

RequestProcesser::RequestProcesser(std::size_t playerId) : m_player_id{ playerId } {}

protocol::serialize::Response RequestProcesser::process(const protocol::serialize::Request &request) const {
  std::cerr << "Read succeed, " << request.size() << std::endl;
  std::cerr << "magic : " << static_cast<int>(request.header.magic) << std::endl;
  std::cerr << "revision : " << request.header.revision << std::endl;
  std::cerr << "size of buffer : " << request.header.bufferSize << std::endl;
  for (auto const &it : request.body.content()) {
    std::cerr << "Body : " << it << std::endl;
  }
  return protocol::make_response(protocol::serialize::StatusCode::OK, {"tes", "vraiment", "trop", "con"});
}

} //namespace space_battle
