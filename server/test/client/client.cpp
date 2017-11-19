//
// Created by Clement on 11/17/2017.
//

//
// Created by 49844 on 12/11/2017.
//

#include <iostream>
#include <string>
#include "client.hpp"
#include "gtest/gtest.h"

class ClientStub {
public:
    ClientStub(protocol::OutputConnection &out) : m_out{ &out } {};
    void run_test() {
        m_out->on_read_success([this] (const protocol::serialize::Response &response, protocol::OutputConnection &) {
            std::cout << "try_connection: Read success, check if user is connected" << std::endl;
            if (response.header.statusCode == protocol::serialize::StatusCode::CLIENT_CONNECTED) {

                std::cout << "try_connection: Connection success" << std::endl;
                auto contentBody = response.body.content();
                ASSERT_EQ(contentBody.size(), static_cast<std::size_t>(2));
                auto idRoom = contentBody[1];
                auto playerId = contentBody[0];
                try {

                    m_room_id = std::stoi(idRoom);
                    m_player_id = std::stoi(playerId);
                    EXPECT_EQ(m_room_id, static_cast<std::size_t>(0));
                    send_message();

                } catch (const std::out_of_range &) {

                    ASSERT_FALSE(true) << "Exception thrown";

                } catch (const std::invalid_argument &) {

                    ASSERT_FALSE(true) << "Exception thrown";

                }

            } else {

                std::cout << "try_connection: cannot connect user !!" << std::endl;
                ASSERT_FALSE(true);

            }
        });
        m_out->write(protocol::make_request(protocol::serialize::OpCode::CLIENT_CONNECT, {"Jean valjean"}));
        m_out->read();
    }
private:
    void send_message() {

        std::cout << "send_message: Trying to send a message to the server" << std::endl;
        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "send_message: Read success, check if the answer is correct" << std::endl;

            auto statusCode = response.header.statusCode;
            ASSERT_EQ(statusCode, protocol::serialize::StatusCode::OK);
            create_game_with_bad_nbr_params();

        });
        m_out->write(protocol::make_request(protocol::serialize::OpCode::SEND_MESSAGE, {std::to_string(m_room_id), "Bonjour la france"}));
        m_out->read();

    }

    void create_game_with_bad_nbr_params() {

        std::cout << "create_game_with_bad_nbr_params: Trying to create a game with only fiew parameters" << std::endl;

        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "create_game_with_bad_nbr_params: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::FORMAT_ERROR);

            std::cout << "create_game_with_bad_nbr_params: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));

            std::cout << "create_game_with_bad_nbr_params: Body size OK" << std::endl;

            std::cout << "create_game_with_bad_nbr_params: Body of the response should contains 0x01" << std::endl;
            ASSERT_EQ(bodyContent[0], "\x01");

            std::cout << "create_game_with_bad_nbr_params: End, everything is OK" << std::endl;
            create_game_with_invalid_visitor();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::CREATE_GAME, {"game01", "1"}));
        m_out->read();
    }

    void create_game_with_invalid_visitor() {

        std::cout << "create_game_with_invalid_visitor: Trying to create a game with invalid number of viewers" << std::endl;

        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "create_game_with_invalid_visitor: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::FORMAT_ERROR);

            std::cout << "create_game_with_invalid_visitor: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));

            std::cout << "create_game_with_invalid_visitor: Body size OK" << std::endl;

            std::cout << "create_game_with_invalid_visitor: Body of the response should contains 0x02" << std::endl;
            ASSERT_EQ(bodyContent[0], "\x02");

            std::cout << "create_game_with_invalid_visitor: End, everything is OK" << std::endl;
            create_game_with_invalid_hasviewer();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::CREATE_GAME, {"game01", "1", "pouet", "0"}));
        m_out->read();
    }

    void create_game_with_invalid_hasviewer() {

        std::cout << "create_game_with_invalid_hasviewer: Trying to create a game with invalid hasviewer" << std::endl;

        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "create_game_with_invalid_hasviewer: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::FORMAT_ERROR);

            std::cout << "create_game_with_invalid_hasviewer: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));

            std::cout << "create_game_with_invalid_hasviewer: Body size OK" << std::endl;

            std::cout << "create_game_with_invalid_hasviewer: Body of the response should contains 0x03" << std::endl;
            ASSERT_EQ(bodyContent[0], "\x03");

            std::cout << "create_game_with_invalid_hasviewer: End, everything is OK" << std::endl;
            create_game_with_bad_timelimit();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::CREATE_GAME, {"game01", "2", "300", "0"}));
        m_out->read();

    }

    void create_game_with_bad_timelimit() {

        std::cout << "create_game_with_bad_timelimit: Trying to create a game with invalid limit of time" << std::endl;

        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "create_game_with_bad_timelimit: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::FORMAT_ERROR);

            std::cout << "create_game_with_bad_timelimit: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));

            std::cout << "create_game_with_bad_timelimit: Body size OK" << std::endl;

            std::cout << "create_game_with_bad_timelimit: Body of the response should contains 0x03" << std::endl;
            ASSERT_EQ(bodyContent[0], "\x04");

            std::cout << "create_game_with_bad_timelimit: End, everything is OK" << std::endl;
            create_game_with_bad_timelimit_conversion();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::CREATE_GAME, {"game01", "1", "300", "300"}));
        m_out->read();

    }

    void create_game_with_bad_timelimit_conversion() {

        std::cout << "create_game_with_bad_timelimit_conversion: Trying to create a game with invalid limit of time conversion" << std::endl;

        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "create_game_with_bad_timelimit_conversion: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::FORMAT_ERROR);

            std::cout << "create_game_with_bad_timelimit_conversion: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));

            std::cout << "create_game_with_bad_timelimit_conversion: Body size OK" << std::endl;

            std::cout << "create_game_with_bad_timelimit_conversion: Body of the response should contains 0x03" << std::endl;
            ASSERT_EQ(bodyContent[0], "\x04");

            std::cout << "create_game_with_bad_timelimit_conversion: End, everything is OK" << std::endl;
            create_game_with_bad_hasviewer_conversion();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::CREATE_GAME, {"game01", "1", "300", "abracadabra"}));
        m_out->read();

    }

    void create_game_with_bad_hasviewer_conversion() {

        std::cout << "create_game_with_invalid_hasviewer_conversion: Trying to create a game with invalid hasviewer conversion" << std::endl;

        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "create_game_with_invalid_hasviewer: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::FORMAT_ERROR);

            std::cout << "create_game_with_invalid_hasviewer_conversion: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));

            std::cout << "create_game_with_invalid_hasviewer_conversion: Body size OK" << std::endl;

            std::cout << "create_game_with_invalid_hasviewer_conversion: Body of the response should contains 0x03" << std::endl;
            ASSERT_EQ(bodyContent[0], "\x03");

            std::cout << "create_game_with_invalid_hasviewer_conversion: End, everything is OK" << std::endl;
            create_game();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::CREATE_GAME, {"game01", "abracadabra", "300", "0"}));
        m_out->read();

    }

    void create_game() {

        std::cout << "create_game: Start creating the game \"game01\" with 3 viewers and no limit of time" << std::endl;

        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "create_game: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            ASSERT_EQ(statusCode, protocol::serialize::StatusCode::GAME_CREATED);
            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));
            m_game_room_id = std::stoi(bodyContent[0]);
            std::cout << "create_game: Game created, get information about it now !" << std::endl;
            get_game_info();

        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::CREATE_GAME, {"game01", "1", "1", "0"}));
        m_out->read();

    }

    void get_game_info() {

        std::cout << "get_game_info: getting information about the game" << std::endl;

        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "get_game_info: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::LIST_GAME);

            std::cout << "get_game_info: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(2));

            std::cout << "get_game_info: Body size OK" << std::endl;

            std::cout << "get_game_info: Check if the room id: " << bodyContent[0] << " is valid" << std::endl;
            auto roomId = std::stoi(bodyContent[0]);
            EXPECT_EQ(roomId, m_game_room_id);

            std::cout << "get_game_info: id of the room OK" << std::endl;

            auto name = bodyContent[1];
            EXPECT_EQ("game01", name);

            std::cout << "get_game_info: name of the game OK" << std::endl;
            get_nbr_client_connected();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::GET_GAME_INFO, {}));
        m_out->read();

    }

    void get_nbr_client_connected() {

        std::cout << "get_nbr_game: Get the number of game, should be one" << std::endl;

        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "get_nbr_game: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::NUMBER_CLIENT_CONNECTED);

            std::cout << "get_nbr_game: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));

            std::cout << "get_nbr_game: Body size OK" << std::endl;

            std::cout << "get_game_info: Check if the number of client: " << bodyContent[0] << " is valid" << std::endl;
            auto nbrClientConnected = std::stoi(bodyContent[0]);
            ASSERT_EQ(nbrClientConnected, static_cast<unsigned>(1));

            std::cout << "get_game_info: End, everything is OK" << std::endl;
            get_nbr_players();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::NBR_PEOPLE_LOGGED_IN, {}));
        m_out->read();

    }

    void get_nbr_players() {

        std::cout << "get_nbr_players: Get the number of game, should be one" << std::endl;

        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "get_nbr_players: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::PLAYER_NUMBER);

            std::cout << "get_nbr_players: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));

            std::cout << "get_nbr_players: Body size OK" << std::endl;

            std::cout << "get_nbr_players: Check if the number of client: " << bodyContent[0] << " is valid" << std::endl;
            auto nbrClientConnected = std::stoi(bodyContent[0]);
            ASSERT_EQ(nbrClientConnected, static_cast<unsigned>(1));

            std::cout << "get_nbr_players: End, everything is OK" << std::endl;
            create_game_without_access();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::PLAYER_LOGGED_IN, {std::to_string(m_game_room_id)}));
        m_out->read();

    }

    void create_game_without_access() {

        std::cout << "create_game_without_access: Get the number of game, should be one" << std::endl;

        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "create_game_without_access: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::PERMISSION_DENIED);

            std::cout << "create_game_without_access: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(0));

            std::cout << "create_game_without_access: Body size OK" << std::endl;

            std::cout << "create_game_without_access: End, everything is OK" << std::endl;
            get_nbr_of_game();
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::CREATE_GAME, {"game01", "1", "3", "0"}));
        m_out->read();

    }

    void get_nbr_of_game() {

        std::cout << "get_nbr_of_game: Get the number of game, should be one" << std::endl;

        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "get_nbr_of_game: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::NUMBER_OF_GAME);

            std::cout << "get_nbr_of_game: Status code OK" << std::endl;

            auto bodyContent = response.body.content();
            ASSERT_EQ(bodyContent.size(), static_cast<std::size_t>(1));

            std::cout << "get_nbr_of_game: Body size OK" << std::endl;

            auto nbrGame = std::stoi(bodyContent[0]);
            ASSERT_EQ(nbrGame, static_cast<unsigned>(1));

            std::cout << "get_nbr_of_game: End, everything is OK" << std::endl;
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::GET_NBR_GAME_CREATED, {}));
        m_out->read();

    }


    protocol::OutputConnection *m_out;
    std::string m_nickname;
    std::size_t m_player_id;
    std::size_t m_room_id;
    std::size_t m_game_room_id;
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
