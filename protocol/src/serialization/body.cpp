//
// Created by 49844 on 12/11/2017.
//

#include "serialization/body.hpp"

namespace protocol {

namespace serialize {

Body::Body(std::initializer_list<std::string> in) noexcept {
    m_body_length = 0;
    for (auto const &content : in) {
        m_body.push_back(content);
        m_body_length += content.size() + 1;
    }
}

Body::Body(const std::vector<std::string> &body) noexcept {
    m_body_length = 0;
    for (auto const &content : body) {
        m_body.push_back(content);
        m_body_length += content.size() + 1;
    }
}

Body::Body(const char *buffer, std::size_t buffer_length) noexcept {
    m_body_length = 0;
    for (std::size_t i = 0; i < buffer_length;) {
        auto str = std::string{&buffer[i]};
        m_body.push_back(str);
        m_body_length += str.size() + 1;
        i += str.size() + 1;
    }
}

std::vector<std::string> const &Body::content() const noexcept {
    return m_body;
}

std::size_t Body::size() const noexcept {
    return m_body_length;
}

void Body::data(char *dest) const noexcept {
    std::size_t i{};
    for (auto const &bodyContent : m_body) {
        for (auto strContent : bodyContent) {
            dest[i] = strContent;
            ++i;
        }
        dest[i] = 0;
        ++i;
    }
}

} // namespace serialize

} // namespace protocol