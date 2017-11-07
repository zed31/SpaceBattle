//
// Created by 49844 on 05/11/2017.
//

#include <iostream>
#include <cstring>
#include "NetIOBusinessData.hpp"
#include "gtest/gtest.h"

static void custom_memcmp(const char *data, const char *compare, std::size_t size) {
    for (std::size_t i = 0; i < size; ++i) {
        //std::cout << "data[" << i << "] = " << data[i] << " compare[" << i << "] = " << compare[i] << std::endl;
        EXPECT_EQ(data[i], compare[i]);
    }
}

static void custom_body_compare(std::vector<std::string> const &data, std::vector<std::string> const &compare) {
    ASSERT_EQ(data.size(), compare.size());
    for (std::size_t i = 0; i < data.size(); ++i) {
        EXPECT_EQ(data[i], compare[i]);
    }
}

TEST(responseHeader, size_of_header) {
    protocol::HeaderResponse h{};
    h.bufferSize = 30;
    h.statusCode = protocol::StatusCode::FORMAT_ERROR;
    h.revision = 1;
    h.magic = 0x2A;

    EXPECT_EQ(h.size(), static_cast<unsigned>(25));

    h.bufferSize = std::numeric_limits<unsigned>::max();
    h.statusCode = protocol::StatusCode::FORMAT_ERROR;

    EXPECT_EQ(h.size(), static_cast<unsigned>(25));
}

TEST(responseHeader, store_header_inside_a_buffer) {
    protocol::HeaderResponse h{};
    h.bufferSize = 2;
    h.statusCode = protocol::StatusCode::CLIENT_ARRIVED_IN_GAME;
    h.revision = 32;
    h.magic = 0x2A;

    auto *data = new char[h.size()];
    h.data(&data[0]);
    custom_memcmp(data, "\x2A\0\x30\x30\x30\x33\x32\0\x34\x35\x33\x32\x39\0\x30\x30\x30\x30\x30\x30\x30\x30\x30\x32\0", h.size());
    delete[] data;

    h.magic = 0x2A;
    h.revision = std::numeric_limits<short>::max();
    h.statusCode = protocol::StatusCode::FORMAT_ERROR;
    h.bufferSize = std::numeric_limits<unsigned>::max();

    data = new char[h.size()];
    h.data(&data[0]);
    custom_memcmp(data, "\x2A\0\x33\x32\x37\x36\x37\0\x34\x35\x30\x36\x30\0\x34\x32\x39\x34\x39\x36\x37\x32\x39\x35\0", h.size());
    delete[] data;
}

TEST(responseHeader, serialize_and_unserialize) {
    protocol::HeaderResponse h{};
    h.bufferSize = 80;
    h.revision = 44;

    auto *data = new char[h.size()];
    h.data(data);
    protocol::HeaderResponse result{data, h.size()};
    EXPECT_EQ(result.magic, 0x2A);
    EXPECT_EQ(result.revision, 44);
    EXPECT_EQ(result.bufferSize, static_cast<unsigned>(80));
    delete[] data;

    result.statusCode = protocol::StatusCode::CLIENT_CONNECTED;
    result.magic = 'a';
    data = new char[result.size()];
    result.data(data);
    protocol::HeaderResponse result2{data, result.size()};
    EXPECT_EQ(result2.magic, 'a');
    EXPECT_EQ(result2.statusCode, protocol::StatusCode::CLIENT_CONNECTED);
    EXPECT_EQ(result2.bufferSize, static_cast<unsigned>(80));
    EXPECT_EQ(result2.revision, 44);
    delete[] data;

    constexpr auto str = "\x2A\0\x30\x30\x30\x30\x34\0\x34\x35\x30\x35\x36\0";
    constexpr std::size_t size = 10;
    protocol::HeaderResponse headerConstruct{ str, size };
    EXPECT_EQ(headerConstruct.magic, 0x2A);
    EXPECT_EQ(headerConstruct.revision, 4);
    EXPECT_EQ(headerConstruct.statusCode, protocol::StatusCode::OK);
    EXPECT_EQ(headerConstruct.bufferSize, static_cast<unsigned>(0));

    constexpr auto str2 = "\x2A\0\x30\x30\x30\x30\x34\0";
    constexpr std::size_t size_str2 = 4;
    protocol::HeaderResponse hC{str2, size_str2};
    EXPECT_EQ(hC.magic, 0x2A);
    EXPECT_EQ(hC.revision, 4);
    EXPECT_EQ(hC.statusCode, protocol::StatusCode::OK);
    EXPECT_EQ(hC.bufferSize, static_cast<unsigned>(0));
}

TEST(requestHeader, size_of_header) {
    protocol::HeaderRequest h{};
    h.bufferSize = 30;
    h.opCode = protocol::OpCode::CREATE_GAME;
    h.revision = 1;
    h.magic = 0x2A;

    EXPECT_EQ(h.size(), static_cast<unsigned>(25));

    h.bufferSize = std::numeric_limits<unsigned>::max();
    h.opCode = protocol::OpCode::CLIENT_DISCONNECT;

    EXPECT_EQ(h.size(), static_cast<unsigned>(25));
}

TEST(requestHeader, store_header_inside_a_buffer) {
    protocol::HeaderRequest h{};
    h.bufferSize = 2;
    h.opCode = protocol::OpCode::NUMBER_GAME_CREATED;
    h.revision = 32;
    h.magic = 0x2A;

    auto *data = new char[h.size()];
    h.data(&data[0]);
    custom_memcmp(data, "\x2A\0\x30\x30\x30\x33\x32\0\x34\x31\x32\x33\x32\0\x30\x30\x30\x30\x30\x30\x30\x30\x30\x32\0", h.size());
    delete[] data;

    h.magic = 0x2A;
    h.revision = std::numeric_limits<short>::max();
    h.opCode = protocol::OpCode::JOIN_AS_VIEWER;
    h.bufferSize = std::numeric_limits<unsigned>::max();

    data = new char[h.size()];
    h.data(&data[0]);
    custom_memcmp(data, "\x2A\0\x33\x32\x37\x36\x37\0\x34\x31\x32\x33\x35\0\x34\x32\x39\x34\x39\x36\x37\x32\x39\x35\0", h.size());
    delete[] data;
}

TEST(requestHeader, serialize_and_unserialize) {
    protocol::HeaderRequest h{};
    h.bufferSize = 80;
    h.revision = 44;

    auto *data = new char[h.size()];
    h.data(data);
    protocol::HeaderRequest result{data, h.size()};
    EXPECT_EQ(result.magic, 0x2A);
    EXPECT_EQ(result.revision, 44);
    EXPECT_EQ(result.bufferSize, static_cast<unsigned>(80));
    delete[] data;

    result.opCode = protocol::OpCode::CREATE_GAME;
    result.magic = 'a';
    data = new char[result.size()];
    result.data(data);
    protocol::HeaderRequest result2{data, result.size()};
    EXPECT_EQ(result2.magic, 'a');
    EXPECT_EQ(result2.opCode, protocol::OpCode::CREATE_GAME);
    EXPECT_EQ(result2.bufferSize, static_cast<unsigned>(80));
    EXPECT_EQ(result2.revision, 44);
    delete[] data;

    constexpr auto str = "\x2A\0\x30\x30\x30\x30\x34\0\x34\x31\x32\x35\x30\0";
    constexpr std::size_t size = 10;
    protocol::HeaderRequest headerConstruct{ str, size };
    EXPECT_EQ(headerConstruct.magic, 0x2A);
    EXPECT_EQ(headerConstruct.revision, 4);
    EXPECT_EQ(headerConstruct.opCode, protocol::OpCode::LEAVE);
    EXPECT_EQ(headerConstruct.bufferSize, static_cast<unsigned>(0));

    constexpr auto str2 = "\x2A\0\x30\x30\x30\x30\x34\0";
    constexpr std::size_t size_str2 = 4;
    protocol::HeaderRequest hC{str2, size_str2};
    EXPECT_EQ(hC.magic, 0x2A);
    EXPECT_EQ(hC.revision, 4);
    EXPECT_EQ(hC.opCode, protocol::OpCode::CLIENT_DISCONNECT);
    EXPECT_EQ(hC.bufferSize, static_cast<unsigned>(0));
}

TEST(Body, size_of_body) {
    protocol::Body b{ "salut", "les", "enfants" };
    EXPECT_EQ(b.size(), static_cast<unsigned>(18));
    protocol::Body b2{ "bonjour" };
    EXPECT_EQ(b2.size(), static_cast<unsigned>(8));
}

TEST(Body, serialization_of_the_body) {
    protocol::Body b{ "bonjour" };
    EXPECT_EQ(b.size(), static_cast<unsigned>(8));

    auto *data = new char[b.size()];
    b.data(&data[0]);
    custom_memcmp(data, "bonjour\0", b.size());
    delete[] data;

    protocol::Body b2{ "bonjour", "les", "enfants" };
    EXPECT_EQ(b2.size(), static_cast<unsigned>(20));
    data = new char[b2.size()];
    b2.data(&data[0]);
    custom_memcmp(data, "bonjour\0les\0enfants\0", b2.size());
}

TEST(Body, reconstruction) {
    constexpr auto body = "bonjour\0les\0enfants\0";
    constexpr std::size_t size = 20;
    protocol::Body b{ body, size };
    custom_body_compare(b.content(), std::vector<std::string>{"bonjour", "les", "enfants"});

    protocol::Body b2{ "dehors", "les", "arabes" };
    EXPECT_EQ(b2.size(), static_cast<unsigned>(18));
    auto *data = new char[b2.size()];
    b2.data(&data[0]);
    custom_memcmp(data, "dehors\0les\0arabes\0", b2.size());

    protocol::Body bCopy{ data, b2.size() };
    custom_body_compare(bCopy.content(), std::vector<std::string>{"dehors", "les", "arabes"});
    delete[] data;
}

TEST(Request, request_deconstruction_and_construction) {
    protocol::Request r{0x2A, 1, protocol::OpCode::LEAVE, 10, {"salut", "les", "moches"}};

    EXPECT_EQ(r.header.size(), static_cast<unsigned>(25));
    EXPECT_EQ(r.body.size(), static_cast<unsigned>(17));
    EXPECT_EQ(r.size(), static_cast<unsigned>(42));

    auto *data = new char[r.size()];
    r.data(&data[0]);

    custom_memcmp(data, "\x2A\0\x30\x30\x30\x30\x31\0\x34\x31\x32\x35\x30\0\x30\x30\x30\x30\x30\x30\x30\x30\x31\x30\0salut\0les\0moches", r.size());

    protocol::Request r2{ data, r.size() };
    auto *headerData = new char[r2.header.size()];
    r2.header.data(&headerData[0]);

    EXPECT_EQ(r.header.size(), static_cast<unsigned>(25));
    EXPECT_EQ(r.body.size(), static_cast<unsigned>(17));
    EXPECT_EQ(r.size(), static_cast<unsigned>(42));

    custom_memcmp(headerData, "\x2A\0\x30\x30\x30\x30\x31\0\x34\x31\x32\x35\x30\0\x30\x30\x30\x30\x30\x30\x30\x30\x31\x30\0", r2.header.size());
    custom_body_compare(r2.body.content(), std::vector<std::string>{"salut", "les", "moches"});

    delete[] headerData;
    delete[] data;
}

TEST(Response, request_deconstruction_and_construction) {
    protocol::Response r{0x2A, 1, protocol::StatusCode::OK, 10, {"salut", "les", "moches"}};

    EXPECT_EQ(r.header.size(), static_cast<unsigned>(25));
    EXPECT_EQ(r.body.size(), static_cast<unsigned>(17));
    EXPECT_EQ(r.size(), static_cast<unsigned>(42));

    auto *data = new char[r.size()];
    r.data(&data[0]);

    custom_memcmp(data, "\x2A\0\x30\x30\x30\x30\x31\0\x34\x35\x30\x35\x36\0\x30\x30\x30\x30\x30\x30\x30\x30\x31\x30\0salut\0les\0moches", r.size());

    protocol::Response r2{ data, r.size() };
    auto *headerData = new char[r2.header.size()];
    r2.header.data(&headerData[0]);

    EXPECT_EQ(r.header.size(), static_cast<unsigned>(25));
    EXPECT_EQ(r.body.size(), static_cast<unsigned>(17));
    EXPECT_EQ(r.size(), static_cast<unsigned>(42));

    custom_memcmp(headerData, "\x2A\0\x30\x30\x30\x30\x31\0\x34\x35\x30\x35\x36\0\x30\x30\x30\x30\x30\x30\x30\x30\x31\x30\0", r2.header.size());
    custom_body_compare(r2.body.content(), std::vector<std::string>{"salut", "les", "moches"});

    delete[] headerData;
    delete[] data;
}
