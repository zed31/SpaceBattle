//
// Created by 49844 on 14/11/2017.
//

#ifndef SPACE_BATTLE_SERVERENGINE_HPP
#define SPACE_BATTLE_SERVERENGINE_HPP

#include "input_connection_pool.hpp"
#include "server.hpp"

namespace space_battle {

/*! \brief used to launch and configure the whole server */
class Engine {
public:
    /*! \brief build the engine, but not run the server
     * @param[in]   port    The port used to bind
    */
    Engine(unsigned short port);

    /*! \brief run the server */
    void run();

    /*! \brief stop the server */
    void stop();
private:
    protocol::server m_server;
    InputConnectionPool m_input_collection;
};

} // namespace space_battle

#endif //SPACE_BATTLE_SERVERENGINE_HPP
