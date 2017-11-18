//
// Created by 49844 on 12/11/2017.
//

#include "serialization/request.hpp"

namespace protocol {

namespace serialize {

HeaderRequest::HeaderRequest()
    : magic{ HeaderUtility::default_magic_ }, revision{ HeaderUtility::default_rev_ },
      opCode{ protocol::serialize::OpCode::CLIENT_DISCONNECT }, bufferSize{ HeaderUtility::default_buffer_size_ } {}

HeaderRequest::HeaderRequest(char m, short r, OpCode o, unsigned b)
    : magic{m}, revision{r}, opCode{o}, bufferSize{b} {}

HeaderRequest::HeaderRequest(const char *buffer, std::size_t buffer_length)
    : magic{ HeaderUtility::default_magic_ }, revision{ HeaderUtility::default_rev_ },
      opCode{ protocol::serialize::OpCode::CLIENT_DISCONNECT }, bufferSize{ HeaderUtility::default_buffer_size_ } {
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
    opCode = static_cast<protocol::serialize::OpCode>(opCodeNbr);
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

void Request::data(char *dest) const {
    auto headerSize = header.size();
    header.data(&dest[0]);
    body.data(&dest[headerSize]);
}

Request::Request(char magic, short revision, OpCode opcode, unsigned bufferSize) noexcept
    : header{magic, revision, opcode, bufferSize}, body{} {}

Request::Request(const char *buffer, std::size_t buffer_length)
    : header{ &buffer[0], buffer_length },
      body{ &buffer[header.size()], buffer_length - header.size() } {
}

Request::Request(char m, short r, OpCode o, unsigned b,
                 std::initializer_list<std::string> bodyContent)
    : header{m, r, o, b}, body{bodyContent}{}

Request::Request(char m, short r, OpCode o, unsigned b,
                 const std::vector<std::string> &bodyContent) : header{m, r, o, b}, body{bodyContent} {}

std::size_t Request::size() const noexcept {
    return header.size() + body.size();
}

} // namespace protocol

serialize::Request make_request(const serialize::OpCode &opCode, std::initializer_list<std::string> body) {
    serialize::Body b{body};
    serialize::Request req{ serialize::HeaderUtility::default_magic_, serialize::HeaderUtility::default_rev_, opCode, b.size(), body };
    return req;
}

serialize::Request make_request(const serialize::OpCode &opCode, const std::vector<std::string> &body) {
    serialize::Body b{body};
    serialize::Request req{ serialize::HeaderUtility::default_magic_, serialize::HeaderUtility::default_rev_, opCode, b.size(), body };
    return req;
}

} // namespace serialization