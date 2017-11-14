//
// Created by 49844 on 14/11/2017.
//

#ifndef SPACE_BATTLE_OUTPUTCONNECTIONPOOL_HPP
#define SPACE_BATTLE_OUTPUTCONNECTIONPOOL_HPP

#include <memory>
#include <vector>
#include "protocol.hpp"
#include "server.hpp"

namespace space_battle {

/*! \brief used to store the OutputConnection */
class InputConnectionPool {
    using output_ptr_t = std::unique_ptr<protocol::InputConnection>;
    using proto_id_t = std::size_t;
public:
    /*! \brief insert an input connection into the collection
     * This class is responsible of the given InputConnection pointer. It returns the id where the connection
     * is stored
     * @param[in]   connection  The connection stored
    */
    proto_id_t insert(output_ptr_t &&connection);

    /*! \brief Remove the connection from the given associated id. It also close the connection
     * @param[in]   id  The id of the InputConnection
    */
    void remove(proto_id_t id);
private:
    struct InputConnectionAssociation {
        proto_id_t id;
        std::unique_ptr<Protocol> input_connection;
    };
    std::vector<InputConnectionAssociation> m_input_collection;
    std::vector<InputConnectionAssociation> m_input_collection_closed;
};

} // namespace space_battle

#endif //SPACE_BATTLE_OUTPUTCONNECTIONPOOL_HPP
