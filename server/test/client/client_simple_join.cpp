//
// Created by Clement on 11/19/2017.
//

#include <iostream>
#include <string>
#include "client.hpp"
#include "gtest/gtest.h"

class ClientStub {
public:
    ClientStub(protocol::OutputConnection &output) : m_out{&output} {}
    void run_test() {
        std::cout << "ClientStub::run_test: Running test, trying to get first game" << std::endl;
        get_first_game();
    }
private:
    void get_first_game() {

        std::cout << "ClientStub::get_first_game: Connect to game01 as viewer" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "ClientStub::get_first_game: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::LIST_GAME);

            std::cout << "ClientStub::get_first_game: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(2));

            std::cout << "ClientStub::get_first_game: Body size OK" << std::endl;

            std::cout << "ClientStub::get_first_game: Body of the response should contains the id as first index: " << bodyContent[0] << std::endl;
            auto gameId = std::stoi(bodyContent[0]);
            m_id = gameId;


            std::cout << "ClientStub::get_first_game: End, everything is OK" << std::endl;
            connect_as_viewer_with_bad_gameid_conversion();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::GET_GAME_INFO, {"1"}));
        m_out->read();

    }

    void connect_as_viewer_with_bad_gameid_conversion() {

        std::cout << "ClientStub::connect_as_viewer_with_bad_gameid_conversion: Connect to game01 as viewer" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "ClientStub::connect_as_viewer_with_bad_gameid_conversion: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::FORMAT_ERROR);

            std::cout << "ClientStub::connect_as_viewer_with_bad_gameid_conversion: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));

            std::cout << "ClientStub::connect_as_viewer_with_bad_gameid_conversion: Body size OK" << std::endl;

            std::cout << "ClientStub::connect_as_viewer_with_bad_gameid_conversion: Body of the response should contains 0x03" << std::endl;
            ASSERT_EQ(bodyContent[0], "\x01");


            std::cout << "ClientStub::connect_as_viewer_with_bad_gameid_conversion: End, everything is OK" << std::endl;
            connect_as_viewer_with_bad_gameid();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::JOIN_AS_VIEWER, {"tototototo"}));
        m_out->read();

    }

    void connect_as_viewer_with_bad_gameid() {

        std::cout << "ClientStub::connect_as_viewer_with_bad_gameid: Connect to game01 as viewer" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "ClientStub::connect_as_viewer_with_bad_gameid: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::ROOM_NOT_FOUND);

            std::cout << "ClientStub::connect_as_viewer_with_bad_gameid: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(0));

            std::cout << "ClientStub::connect_as_viewer_with_bad_gameid: Body size OK" << std::endl;

            std::cout << "ClientStub::connect_as_viewer_with_bad_gameid: End, everything is OK" << std::endl;
            connect_as_player_with_bad_gameid_conversion();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::JOIN_AS_VIEWER, {"3000"}));
        m_out->read();

    }

    void connect_as_player_with_bad_gameid_conversion() {

        std::cout << "ClientStub::connect_as_player_with_bad_gameid_conversion: Connect to game01 as viewer" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "ClientStub::connect_as_player_with_bad_gameid_conversion: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::FORMAT_ERROR);

            std::cout << "ClientStub::connect_as_player_with_bad_gameid_conversion: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));

            std::cout << "ClientStub::connect_as_player_with_bad_gameid_conversion: Body size OK" << std::endl;

            std::cout << "ClientStub::connect_as_player_with_bad_gameid_conversion: Body of the response should contains 0x03" << std::endl;
            ASSERT_EQ(bodyContent[0], "\x01");


            std::cout << "ClientStub::connect_as_player_with_bad_gameid_conversion: End, everything is OK" << std::endl;
            connect_as_player_with_bad_gameid();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::JOIN_AS_PLAYER, {"tototototo"}));
        m_out->read();

    }

    void connect_as_player_with_bad_gameid() {

        std::cout << "ClientStub::connect_as_player_with_bad_gameid: Connect to game01 as viewer" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "ClientStub::connect_as_player_with_bad_gameid: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::ROOM_NOT_FOUND);

            std::cout << "ClientStub::connect_as_player_with_bad_gameid: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(0));

            std::cout << "ClientStub::connect_as_player_with_bad_gameid: Body size OK" << std::endl;

            std::cout << "ClientStub::connect_as_player_with_bad_gameid: End, everything is OK" << std::endl;
            connect_as_player();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::JOIN_AS_PLAYER, {"3000"}));
        m_out->read();

    }

    void connect_as_player() {

        std::cout << "ClientStub::connect_as_player: Connect to game01 as viewer" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "ClientStub::connect_as_player: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            ASSERT_EQ(statusCode, protocol::serialize::StatusCode::CLIENT_JOIN_GAME);

            std::cout << "ClientStub::connect_as_player: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(5));

            std::cout << "ClientStub::connect_as_player: Body size OK" << std::endl;

            std::cout << "ClientStub::connect_as_player: End, everything is OK" << std::endl;
            connect_as_viewer_bad_permission();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::JOIN_AS_PLAYER, {std::to_string(m_id)}));
        m_out->read();

    }

    void connect_as_viewer_bad_permission() {

        std::cout << "ClientStub::connect_as_viewer_bad_permission: Connect to game01 as viewer" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "ClientStub::connect_as_viewer_bad_permission: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            ASSERT_EQ(statusCode, protocol::serialize::StatusCode::PERMISSION_DENIED);

            std::cout << "ClientStub::connect_as_viewer_bad_permission: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(0));

            std::cout << "ClientStub::connect_as_viewer_bad_permission: Body size OK" << std::endl;

            std::cout << "ClientStub::connect_as_viewer_bad_permission: End, everything is OK" << std::endl;
            connect_as_player_bad_permission();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::JOIN_AS_VIEWER, {std::to_string(m_id)}));
        m_out->read();

    }

    void connect_as_player_bad_permission() {

        std::cout << "ClientStub::connect_as_player_bad_permission: Connect to game01 as viewer" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "ClientStub::connect_as_player_bad_permission: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            ASSERT_EQ(statusCode, protocol::serialize::StatusCode::PERMISSION_DENIED);

            std::cout << "ClientStub::connect_as_player_bad_permission: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(0));

            std::cout << "ClientStub::connect_as_player_bad_permission: Body size OK" << std::endl;

            std::cout << "ClientStub::connect_as_player_bad_permission: End, everything is OK" << std::endl;
            leave_room();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::JOIN_AS_PLAYER, {std::to_string(m_id)}));
        m_out->read();


    }

    void leave_room() {

        std::cout << "ClientStub::leave_room: Connect to game01 as viewer" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "ClientStub::leave_room: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            ASSERT_EQ(statusCode, protocol::serialize::StatusCode::LEAVE_SUCCESS);

            std::cout << "ClientStub::leave_room: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(0));

            std::cout << "ClientStub::leave_room: Body size OK" << std::endl;

            std::cout << "ClientStub::leave_room: End, everything is OK" << std::endl;
            connect_as_viewer();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::LEAVE, {}));
        m_out->read();


    }

    void connect_as_viewer() {

        std::cout << "ClientStub::connect_as_viewer: Connect to game01 as viewer" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "ClientStub::connect_as_viewer: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            ASSERT_EQ(statusCode, protocol::serialize::StatusCode::CLIENT_JOIN_GAME);

            std::cout << "ClientStub::connect_as_viewer: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(5));

            std::cout << "ClientStub::connect_as_viewer: Body size OK" << std::endl;

            std::cout << "ClientStub::connect_as_viewer: End, everything is OK" << std::endl;
            get_game_status();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::JOIN_AS_VIEWER, {std::to_string(m_id)}));
        m_out->read();

    }

    void get_game_status() {

        std::cout << "ClientStub::get_game_status:" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "ClientStub::get_game_status: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::GAME_STATUS);

            std::cout << "ClientStub::get_game_status: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));

            std::cout << "ClientStub::get_game_status: Body size OK" << std::endl;

            std::cout << "ClientStub::get_game_status: Body of the response should contains New status: " << bodyContent[0] << std::endl;
            auto status = std::stoi(bodyContent[0]);
            auto statusSerialized = static_cast<protocol::serialize::GameStatus>(status);
            ASSERT_EQ(statusSerialized, protocol::serialize::GameStatus::NEW);


            std::cout << "ClientStub::get_game_status: End, everything is OK" << std::endl;
            get_game_status_bad_param();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::GET_GAME_STATUS, {std::to_string(m_id)}));
        m_out->read();

    }

    void get_game_status_bad_param() {

        std::cout << "ClientStub::get_game_status_bad_param:" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "ClientStub::get_game_status: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::FORMAT_ERROR);

            std::cout << "ClientStub::get_game_status_bad_param: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));

            std::cout << "ClientStub::get_game_status_bad_param: Body size OK" << std::endl;

            std::cout << "ClientStub::get_game_status_bad_param: Body of the response should contains 0x01" << std::endl;
            ASSERT_EQ(bodyContent[0], "\x01");


            std::cout << "ClientStub::get_game_status_bad_param: End, everything is OK" << std::endl;
            get_game_status_bad_room();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::GET_GAME_STATUS, {"ababababab"}));
        m_out->read();

    }

    void get_game_status_bad_room() {

        std::cout << "ClientStub::get_game_status_bad_room:" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "ClientStub::get_game_status_bad_room: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::ROOM_NOT_FOUND);

            std::cout << "ClientStub::get_game_status_bad_room: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(0));

            std::cout << "ClientStub::get_game_status_bad_room: Body size OK" << std::endl;


            std::cout << "ClientStub::get_game_status_bad_room: End, everything is OK" << std::endl;
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::GET_GAME_STATUS, {"33"}));
        m_out->read();


    }

    protocol::OutputConnection *m_out;
    std::size_t m_id;
};

int main() {
    try {
        protocol::client client{ "127.0.0.1", "6060" };
        std::unique_ptr<ClientStub> stub;

        auto on_connect = [&](protocol::OutputConnection &outputConnection) {

            stub = std::make_unique<ClientStub>(outputConnection);
            stub->run_test();

        };

        auto on_connection_fails = [&]() {
            std::cerr << "on_connection_fails: Stopping the connection" << std::endl;
            client.stop();
        };

        client.on_connect(on_connect);

        client.on_connection_failed(on_connection_fails);
        std::thread t(&protocol::client::run, &client);
        t.join();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}