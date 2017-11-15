//
// Created by Clement on 11/15/2017.
//

#ifndef SPACE_BATTLE_CHAT_ROOM_HPP
#define SPACE_BATTLE_CHAT_ROOM_HPP

#include <string>
#include <vector>
#include <serialization/data_utils.hpp>

namespace space_battle {

class ChatRoom {
public:
    /*! \brief Structure that contains information about the chat */
    struct ChatInformation {
        const std::size_t playerId;
        const std::string message;
    };

    /*! \brief Insert a message in the chatroom
     * @param[in]   playerId    the id of the player
     * @param[in]   message     the message
     * Return the status code depending on how the function goes on
    */
    protocol::serialize::StatusCode insert_message(std::size_t const playerId, const std::string &message);

    /*! \brief clean the chat room */
    void clean();

    /*! \brief return the chat information */
    std::vector<ChatInformation> const &get_chat_information() const;
private:
    std::vector<ChatInformation> m_chat;
};

} // namespace space_battle

#endif //SPACE_BATTLE_CHAT_ROOM_HPP
