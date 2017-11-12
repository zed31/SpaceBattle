//
// Created by 49844 on 06/11/2017.
//

#include "NetIOBusinessData.hpp"

namespace protocol {

HeaderResponse::HeaderResponse(char m, short r, StatusCode s, unsigned b)
    : magic{m}, revision{r}, statusCode{s}, bufferSize{b} {}

HeaderResponse::HeaderResponse()
    : magic{ HeaderUtility::default_magic_ }, revision{ HeaderUtility::default_rev_ },
      statusCode{ protocol::StatusCode::OK }, bufferSize{ HeaderUtility::default_buffer_size_ } {}

HeaderResponse::HeaderResponse(const char *buffer, std::size_t buffer_length)
    : magic{ HeaderUtility::default_magic_ }, revision{ HeaderUtility::default_rev_ },
      statusCode{ protocol::StatusCode::OK }, bufferSize{ HeaderUtility::default_buffer_size_ } {
    if (buffer_length > 0) {
        magic = buffer[0];
        std::size_t idx{ 2 };
        fill_revision(idx, buffer, buffer_length);
        fill_status_code(idx, buffer, buffer_length);
        fill_buffer_size(idx, buffer, buffer_length);
    }
}

void HeaderResponse::fill_revision(std::size_t &idx, const char *buffer, std::size_t buffer_length) {
    if (idx >= buffer_length) {
        return ;
    }
    auto revStr = std::string{ &buffer[idx] };
    auto maxBoundary = std::numeric_limits<short>::max();
    auto revNbr = std::stoi(revStr);
    if (revNbr > maxBoundary) {
        throw std::out_of_range{ "Error, revision number out of range" };
    }
    revision = static_cast<short>(revNbr);
    idx += revStr.size() + 1;
}

void HeaderResponse::fill_status_code(std::size_t &idx, const char *buffer, std::size_t buffer_length) {
    if (idx >= buffer_length) {
        return ;
    }
    auto statusCodeStr = std::string{ &buffer[idx] };
    auto maxBoundary = std::numeric_limits<unsigned short>::max();
    auto statusCodeNbr = std::stoi(statusCodeStr);
    if (statusCodeNbr > maxBoundary) {
        throw std::out_of_range{ "Error : Status code is out of range" };
    }
    statusCode = static_cast<protocol::StatusCode>(statusCodeNbr);
    idx += statusCodeStr.size() + 1;
}

void HeaderResponse::fill_buffer_size(std::size_t &idx, const char *buffer, std::size_t buffer_length) {
    if (idx >= buffer_length) {
        return ;
    }
    auto bufferSizeStr = std::string{ &buffer[idx] };
    bufferSize = static_cast<unsigned>(std::stoi(bufferSizeStr));
    idx += bufferSizeStr.size() + 1;
}

std::size_t HeaderResponse::size() const {
    auto revisionStr = HeaderUtility::convert(revision);
    auto statusCodeStr = HeaderUtility::convert(static_cast<unsigned short>(statusCode));
    auto bufferSizeStr = HeaderUtility::convert(bufferSize);
    return sizeof(char) + revisionStr.size() + statusCodeStr.size() + bufferSizeStr.size() + 4;
}

void HeaderResponse::data(char *dest) const {
    auto revisionStr = HeaderUtility::convert(revision);
    auto statusCodeStr = std::to_string(static_cast<unsigned short>(statusCode));
    auto bufferSizeStr = HeaderUtility::convert(bufferSize);

    dest[0] = magic;
    dest[1] = 0;
    std::size_t idx{ 2 };
    std::copy(revisionStr.begin(), revisionStr.end(), &dest[idx]);
    idx += revisionStr.size();
    dest[idx] = 0;
    ++idx;
    std::copy(statusCodeStr.begin(), statusCodeStr.end(), &dest[idx]);
    idx += statusCodeStr.size();
    dest[idx] = 0;
    ++idx;
    std::copy(bufferSizeStr.begin(), bufferSizeStr.end(), &dest[idx]);
    idx += bufferSizeStr.size();
    dest[idx] = 0;
}

HeaderRequest::HeaderRequest()
    : magic{ HeaderUtility::default_magic_ }, revision{ HeaderUtility::default_rev_ },
      opCode{ protocol::OpCode::CLIENT_DISCONNECT }, bufferSize{ HeaderUtility::default_buffer_size_ } {}

HeaderRequest::HeaderRequest(char m, short r, OpCode o, unsigned b)
    : magic{m}, revision{r}, opCode{o}, bufferSize{b} {}

HeaderRequest::HeaderRequest(const char *buffer, std::size_t buffer_length)
    : magic{ HeaderUtility::default_magic_ }, revision{ HeaderUtility::default_rev_ },
      opCode{ protocol::OpCode::CLIENT_DISCONNECT }, bufferSize{ HeaderUtility::default_buffer_size_ } {
    if (buffer_length > 0) {
        magic = buffer[0];
        std::size_t idx{ 2 };
        fill_revision(idx, buffer, buffer_length);
        fill_op_code(idx, buffer, buffer_length);
        fill_buffer_size(idx, buffer, buffer_length);
    }
}

void HeaderRequest::fill_revision(std::size_t &idx, const char *buffer, std::size_t buffer_length) {
    if (idx >= buffer_length) {
        return ;
    }
    auto revStr = std::string{ &buffer[idx] };
    auto maxBoundary = std::numeric_limits<short>::max();
    auto revNbr = std::stoi(revStr);
    if (revNbr > maxBoundary) {
        throw std::out_of_range{ "Error, revision number out of range" };
    }
    revision = static_cast<short>(revNbr);
    idx += revStr.size() + 1;
}

void HeaderRequest::fill_op_code(std::size_t &idx, const char *buffer, std::size_t buffer_length) {
    if (idx >= buffer_length) {
        return ;
    }
    auto statusCodeStr = std::string{ &buffer[idx] };
    auto maxBoundary = std::numeric_limits<unsigned short>::max();
    auto opCodeNbr = std::stoi(statusCodeStr);
    if (opCodeNbr > maxBoundary) {
        throw std::out_of_range{ "Error : Status code is out of range" };
    }
    opCode = static_cast<protocol::OpCode>(opCodeNbr);
    idx += statusCodeStr.size() + 1;
}

void HeaderRequest::fill_buffer_size(std::size_t &idx, const char *buffer, std::size_t buffer_length) {
    if (idx >= buffer_length) {
        return ;
    }
    auto bufferSizeStr = std::string{ &buffer[idx] };
    bufferSize = static_cast<unsigned>(std::stoi(bufferSizeStr));
    idx += bufferSizeStr.size() + 1;
}

std::size_t HeaderRequest::size() const {
    auto revisionStr = HeaderUtility::convert(revision);
    auto opCodeStr = std::to_string(static_cast<unsigned short>(opCode));
    auto bufferSizeStr = HeaderUtility::convert(bufferSize);
    return sizeof(char) + revisionStr.size() + opCodeStr.size() + bufferSizeStr.size() + 4;
}

void HeaderRequest::data(char *dest) const {
    auto revisionStr = HeaderUtility::convert(revision);
    auto opCodeStr = std::to_string(static_cast<unsigned short>(opCode));
    auto bufferSizeStr = HeaderUtility::convert(bufferSize);

    dest[0] = magic;
    dest[1] = 0;
    std::size_t idx{ 2 };
    std::copy(revisionStr.begin(), revisionStr.end(), &dest[idx]);
    idx += revisionStr.size();
    dest[idx] = 0;
    ++idx;
    std::copy(opCodeStr.begin(), opCodeStr.end(), &dest[idx]);
    idx += opCodeStr.size();
    dest[idx] = 0;
    ++idx;
    std::copy(bufferSizeStr.begin(), bufferSizeStr.end(), &dest[idx]);
    idx += bufferSizeStr.size();
    dest[idx] = 0;
}

Body::Body(std::initializer_list<std::string> in) noexcept {
    m_body_length = 0;
    for (auto const &content : in) {
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

void Request::data(char *dest) const {
    auto headerSize = header.size();
    header.data(&dest[0]);
    if (body.size()) {
        body.data(&dest[headerSize]);
    }
}

Request::Request(char magic, short revision, OpCode opcode, unsigned bufferSize) noexcept
    : header{magic, revision, opcode, bufferSize} {}

Request::Request(const char *buffer, std::size_t buffer_length)
    : header{ &buffer[0], buffer_length },
      body{ &buffer[header.size()], buffer_length - header.size() } {
}

Request::Request(char m, short r, OpCode o, unsigned b,
                 std::initializer_list<std::string> bodyContent)
    : header{m, r, o, b}, body{bodyContent}{}

std::size_t Request::size() const noexcept {
    return header.size() + body.size();
}

void Response::data(char *dest) const {
    auto headerSize = header.size();
    header.data(&dest[0]);
    body.data(&dest[headerSize]);
}

std::size_t Response::size() const noexcept {
    return header.size() + body.size();
}

Response::Response(char magic, short revision, StatusCode statusCode, unsigned bufferSize) noexcept
    : header{magic, revision, statusCode, bufferSize}, body{} {}

Response::Response(const char *buffer, std::size_t buffer_length)
    : header{ &buffer[0], buffer_length },
      body{ &buffer[header.size()], buffer_length - header.size() } {
}

Response::Response(char m, short r, StatusCode s, unsigned b,
                   std::initializer_list<std::string> bodyContent)
    : header{m, r, s, b}, body{bodyContent}{}

} // namespace protocol