//
// Created by 49844 on 04/11/2017.
//

#ifndef SPACE_BATTLE_SIMPLE_CLIENT_HH
#define SPACE_BATTLE_SIMPLE_CLIENT_HH

#include "asio.hpp"

using asio::ip::tcp;

namespace test {

class SimpleClient {
public:
    SimpleClient(asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator);
    void stop();
private:
    asio::io_service& m_io_service;
    tcp::socket m_socket;
};

}

#endif //SPACE_BATTLE_SIMPLE_CLIENT_HH
