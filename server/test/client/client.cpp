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
    void try_connection() {
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
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::OK);
            if (statusCode == protocol::serialize::StatusCode::OK) {
                std::cout << "send_message: Message sent correctly creating game" << std::endl;
                create_game();
            } else {
                std::cout << "send_message: Message not sent correctly" << std::endl;
            }
        });
        m_out->write(protocol::make_request(protocol::serialize::OpCode::SEND_MESSAGE, {std::to_string(m_room_id), "Bonjour la france"}));
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

        m_out->write(protocol::make_request(protocol::serialize::OpCode::CREATE_GAME, {"game01", "1", "3", "0"}));
        m_out->read();

    }

    void get_game_info() {

        std::cout << "get_game_info: getting information about the game" << std::endl;

        m_out->on_read_success([this](const auto &response, const auto &) {
            std::cout << "get_game_info: Reading something, should be Okay" << std::endl;

            auto statusCode = response.header.statusCode;
            EXPECT_EQ(statusCode, protocol::serialize::StatusCode::GAME_INFO);

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
        });

        m_out->write(protocol::make_request(protocol::serialize::OpCode::GET_GAME_INFO, {}));
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
            stub->try_connection();
            /*auto on_write_success = [&](protocol::OutputConnection &out, const protocol::serialize::Request &request) {
                std::cout << "on_write_success: Writing success, processing read now" << std::endl;
                out.read();
            };
            outputConnection.on_write_success(on_write_success);

            auto on_write_failure = [&](std::error_code ec, protocol::OutputConnection &out, const protocol::serialize::Request &) {
                std::cerr << "on_write_failure: " << ec << std::endl;
                out.close();
                client.stop();
            };
            outputConnection.on_write_failure(on_write_failure);

            auto on_read_failure = [&](std::error_code ec, protocol::OutputConnection &out) {
                std::cerr << "on_read_failure: " << ec.message() << std::endl;
                out.close();
            };
            outputConnection.on_read_failure(on_read_failure);

            auto on_read_success = [&] (const protocol::serialize::Response &response, protocol::OutputConnection &out) {

                std::cout << "on_read_success: Reading something" << std::endl;
                if (response.header.statusCode == protocol::serialize::StatusCode::CLIENT_CONNECTED) {

                    std::cout << "on_read_success: Setting the new read function" << std::endl;
                    auto playerId = response.body.content()[0];
                    auto roomId = response.body.content()[1];
                    auto on_read_send_msg_success = [&] (const protocol::serialize::Response &response, protocol::OutputConnection &out) {

                        if (response.header.statusCode == protocol::serialize::StatusCode::OK) {
                            std::cout << "Send message: Success" << std::endl;
                        }

                    };

                    out.on_read_success(on_read_send_msg_success);
                    std::cout << "on_read_success: Send a simple message now" << std::endl;
                    auto request = protocol::make_request(protocol::serialize::OpCode::SEND_MESSAGE, {roomId, "BONJOUR LEL"});
                    out.write(request);
                }
            };
            outputConnection.on_read_success(on_read_success);

            auto on_close = [&] (protocol::OutputConnection &) {
                std::cerr << "on_close: Stoping the server" << std::endl;
                client.stop();
            };
            outputConnection.on_close(on_close);*/

            //only for testing the app !
            //auto request = protocol::make_request(protocol::serialize::OpCode::CLIENT_CONNECT, {"Gregorie"});
            //outputConnection.write(request);
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
