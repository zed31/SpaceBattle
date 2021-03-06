//
// Created by 49844 on 14/11/2017.
//

#ifndef SPACE_BATTLE_PROTOCOL_HPP
#define SPACE_BATTLE_PROTOCOL_HPP

#include <memory>
#include "server.hpp"
#include "room_interface.hpp"
#include "request_processer.hpp"

namespace space_battle {

class InputConnectionPool;

/*! \brief This class store InputConnections */
class Protocol {
    using output_ptr_t = std::unique_ptr<protocol::InputConnection>;
public:
    /*! \brief Construct the protocol with an id and a connection
     * This class is responsible of the destruction of the connection object. It also set the callback
     * to handle the return of each operations for each connection
     * @param[in]   inputCollection The collection of input created
     * @param[in]   id  The id of the connection
     * @param[in]   connection  The pointer of the connection moved
     * @param[in]   roomInterface   The room interface
    */
    Protocol(InputConnectionPool &inputCollection, std::size_t id, output_ptr_t &&connection,
             RoomInterface &roomInterface);

    /*! \brief run the IO operation on the socket */
    void run();
private:
    /*! \brief function called when the InputConnection get something from the client
     * @param[in]   request The request that the client sent
     * @param[in]   input   The input associated to this client
    */
    void on_read_success(const protocol::serialize::Request &request, protocol::InputConnection &input);

    /*! \brief Function called when the read operation fail
     * @param[in]   input   The input used to make IO operation on the socket
     * @param[in]   error   The error code
    */
    void on_read_fail(const std::error_code error, protocol::InputConnection &input);

    /*! \brief Function triggered when a write succeed
     * @param[in]   response    The response that the server sent to the client
     * @param[in]   input       The input used to communicate with the client
    */
    void on_write_success(const protocol::serialize::Response &response, protocol::InputConnection &input);
private:
    std::size_t m_id;
    output_ptr_t m_connection;
    InputConnectionPool *m_input_collection;
    RoomInterface *m_room_interface;
    std::unique_ptr<RequestProcesser> m_request_processer;
};

}

#endif //SPACE_BATTLE_PROTOCOL_HPP
