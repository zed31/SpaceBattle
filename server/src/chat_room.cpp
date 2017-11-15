//
// Created by Clement on 11/15/2017.
//

#include "chat_room.hpp"

namespace space_battle {

protocol::serialize::StatusCode ChatRoom::insert_message(std::size_t const playerId, const std::string &message) {
    ChatInformation chatMessage{ playerId, message };
    m_chat.push_back(chatMessage);
    return protocol::serialize::StatusCode::OK;
}

void ChatRoom::clean() {
    m_chat.clear();
}

std::vector<ChatRoom::ChatInformation> const &ChatRoom::get_chat_information() const {
    return m_chat;
}

} // namespace space_battle
