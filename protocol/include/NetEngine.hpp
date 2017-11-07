//
// Created by 49844 on 04/11/2017.
//

#ifndef SPACE_BATTLE_NETENGINE_H
#define SPACE_BATTLE_NETENGINE_H

#include <memory>
#include "Net.hpp"

namespace protocol {

namespace engine {

template<typename NetConnector, typename NetIO, typename = void>
class NetEngine;

template<typename T>
using enable_type = typename std::enable_if<std::is_base_of<protocol::Net, T>::value>::type

template<typename NetConnector, typename NetIO>
class NetEngine<NetConnector, NetIO, enable_type<NetConnector>> {
public:
private:
};

} // namespace engine

} // namespace protocol

#endif //SPACE_BATTLE_NETENGINE_H
