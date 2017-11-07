//
// Created by 49844 on 04/11/2017.
//

#ifndef SPACE_BATTLE_NETIOBUSINESS_HPP
#define SPACE_BATTLE_NETIOBUSINESS_HPP

#include "Net.hpp"
#include <limits>
#include <algorithm>
#include <string>
#include <initializer_list>
#include <memory>
#include <vector>
#include <string>

namespace protocol {

/*! \brief This enumeration contain all the op code possible to send to the server */
enum class OpCode : unsigned short {
    SEND_MESSAGE = 0xA100, /*!< When the client send a message to the chatroom */
    CLIENT_DISCONNECT = 0xA101, /*!< When a client disconnect itself from the server */
    NUMBER_GAME_MAKABLE = 0xA102, /*!< When the client asks for the number of game that he can make at the moment */
    NUMBER_GAME_CREATED = 0xA110, /*!< When the client asks for the number of game created at the moment */
    CREATE_GAME = 0xA111, /*!< When the client wants to create a game */
    JOIN_AS_PLAYER = 0xA112, /*!< When the client wants to join a game as player */
    JOIN_AS_VIEWER = 0xA113, /*!< When the client wants to join a game as viewer */
    GET_GAME_INFO = 0xA114, /*!< When the client wants information about a special game */
    PEOPLE_LOGGED_IN = 0xA115, /*!< When the client wants a list of the people connected at the moment */
    NBR_PEOPLE_LOGGED_IN = 0xA116, /*!< When the client wants the number of people connected at the moment */
    READY_TO_PLAY = 0xA120, /*!< When the client placed all this ship and he is ready to play */
    TARGET = 0xA121, /*!< When the player wants to aim at a specific position of his opponent */
    LEAVE = 0xA122, /*!< When the client wants to leave a game */
    REPLAY = 0xA123, /*!< When the player wants to replay */
    REFUSE_REPLAY = 0xA124, /*!< When the player refuse to replay */
    PLACE_SHIP = 0xA125 /*!< When the player wants to place a ship */
};

/*! \brief This enumeration contain all the response possible the server can send */
enum class StatusCode : unsigned short {
    OK = 0xB000, /*!< When everything is correct and the client expect nothing but a confirmation */
    PERMISSION_DENIED = 0xB001, /*!< When the user can't send the request */
    CLIENT_CONNECTED = 0xB003, /*!< When user just connect to the server */
    CONNECTION_REFUSED = 0xB002, /*!< When a user cannot be connected to the server */
    FORMAT_ERROR = 0xB004, /*!< When there is an error on the client request (on body most of the time) */
    CLIENT_DISCONNECT = 0xB005, /*!< Broadcast to all clients in the room when client disconnect from the server */
    EMPTY_BODY_FAILURE = 0xB006, /*!< When the body should'nt be empty, but he is */
    INVALID_BODY_SIZE = 0xB007, /*!< When the size of the body is invalid */
    INVALID_MAGIC_CODE = 0xB008, /*!< When the magic code of the header is invalid */
    INVALID_REVISION_NUMBER = 0xB009, /*!< When the revision number of the header is invalid */
    INVALID_OPCODE = 0xB010, /*!< When the Opcode is invalid */
    NUMBER_OF_CLIENT_MAX_REACHED = 0xB011, /*!< When the Opcode of the request is invalid */
    NUMBER_GAME_MAKABLE = 0xB012, /*!< Send the number of game the client can make at the moment */
    LIST_GAME = 0xB113, /*!< Send a list of the games to the client */
    GAME_CREATED = 0xB101, /*!< Send game information about the game that the client created */
    TOO_MANY_CLIENTS = 0xB100, /*!< When a client try to join a game (as viewer or player) and there is too many clients connected to it */
    GAME_ID_DOESNT_EXIST = 0xB103, /*!< When the game id requested by the client doesn't exist */
    CLIENT_JOIN_GAME = 0xB102, /*!< Broadcast to all the client on a room when a client join a game */
    GAME_INFO = 0xB105, /*!< Send the information about a game to the corresponding client */
    NICKNAME_PLAYER_LOGGED_IN = 0xB106, /*!< Send the nickname of all the client connected to the general room */
    PLAYER_NUMBER = 0xB107, /*!< Send the number of the player connected to the general room */
    NUMBER_OF_CLIENT_CONNECTABLE = 0xB108, /*!< Send the number of client that the server can still accept */
    GAME_WILL_DESTROY = 0xB110, /*!< Broadcast to all the client in the game that the game will be destroyed */
    CLIENT_ARRIVED_IN_GAME = 0xB111, /*!< Send to all the client in the game that a new client arrive */
    GAME_CHANGE_STATUS = 0xB112, /*!< Send to all the client in a game that the game has changed his status */
    WAITING_FOR_SECOND_PLAYER = 0xB120, /*!< Send to the player who accept to play that the second player has not confirm yet */
    READY_TO_PLAY = 0xB121, /*!< Send that both player has accept and both are ready to play */
    MISS_SHIP = 0xB122, /*!< Broadcast to all clients in the game when a player miss a ship */
    HIT_SHIP = 0xB123, /*!< Broadcast to all clients in the game when a player hit a ship */
    SUNK_SHIP = 0xB212, /*!< Broadcast to all clients in the game when a player sunk a ship */
    PLAYER_WIN = 0xB213 /*!< Broadcast to all clients in the game when a player win */
};

struct HeaderUtility {
    static constexpr short default_rev_ = 1;
    static constexpr char default_magic_ = 0x2A;
    static constexpr unsigned default_buffer_size_ = 0;
    template<typename Nbr>
    static std::string convert(Nbr value) {
        auto maxBoundary = std::to_string(std::numeric_limits<Nbr>::max());
        auto valueStr = std::to_string(value);
        if (valueStr.size() < maxBoundary.size()) {
            auto paddedStr = std::string(maxBoundary.size() - valueStr.size(), '0') + valueStr;
            return paddedStr;
        }
        return valueStr;
    };
};

/*! \brief This class store the body and provide a way to transform it into char * */
class Body {
public:
    /*! \brief Default construction of the Body class */
    Body() = default;

    /*! \brief Construct the body with a set of data
     * @param[in] in    the initializer list used to generate the body
    */
    Body(std::initializer_list<std::string> in) noexcept;

    /*! \brief Construct the body with an array of bytes
     * @param[in]   buffer          The buffer used to construct the body
     * @param[in]   buffer_length   The length of the buffer
    */
    Body(const char *buffer, std::size_t buffer_length) noexcept;

    /*! \brief set all the data contained in the body separate with a \0
     * @param[out]   dest    the destination where the body has to be put in. It must allocate enough memory
     * the data function is not responsible of the size of dest.
    */
    void data(char *dest) const noexcept;

    /*! \brief return the size of the body */
    std::size_t size() const noexcept;

    /*! \brief return the content of the body */
    std::vector<std::string> const &content() const noexcept;
private:
    std::vector<std::string> m_body;
    std::size_t m_body_length;
};

/*! \brief This structure contains the header of a response sent by the server */
struct HeaderResponse {
    char magic; /*!< Magic number of the header (0x2A) */
    short revision; /*!< Revision of the request */
    StatusCode statusCode; /*!< status code containing the response */
    unsigned bufferSize; /*!< Size of the body */

    /*! \brief Default constructor of the HeaderResponse structure */
    HeaderResponse();

    /*! \brief Construct a header request with all the data necessary
     * @param[in]   magic       the magic number of the request
     * @param[in]   revision    the revision of the request
     * @param[in]   statusCode  the opcode of the request
     * @param[in]   bufferSize  the size of the buffer
    */
    HeaderResponse(char magic, short revision, StatusCode statusCode, unsigned bufferSize);

    /*! \brief Construct the response with a buffer.
     * Construct all the response structure with a buffer, if a parameter is missing, then
     * the parameter is set to default value
     * @param[in]   buffer          The buffer used to construct the structure
     * @param[in]   buffer_length   Then length of the buffer
    */
    HeaderResponse(const char *buffer, std::size_t buffer_length);

    /*! \brief Set all the datas in the body separate by a \0
     * @param[out]   dest    The destination of the data, the data function is not responsible of the
     * allocation of dest, you have to make sure that the dest is correctly set.
    */
    void data(char *dest) const;

    /*! \brief Return the size of the header */
    std::size_t size() const;
private:
    void fill_revision(std::size_t &idx, const char *buffer, std::size_t buffer_length);
    void fill_status_code(std::size_t &idx, const char *buffer, std::size_t buffer_length);
    void fill_buffer_size(std::size_t &idx, const char *buffer, std::size_t buffer_length);
};

/*! \brief This structure contains the header of a request sent by the client */
struct HeaderRequest {
    char magic; /*!< Magic number of the header request (0x2A) */
    short revision; /*!< Revision of the response */
    OpCode opCode; /*!< Op code containing the request */
    unsigned bufferSize; /*!< size of the body */

    /*! \brief Default constructor of the HeaderRequest structure */
    HeaderRequest();

    /*! \brief Construct a header request with all the data necessary
     * @param[in]   magic       the magic number of the request
     * @param[in]   revision    the revision of the request
     * @param[in]   opcode      the opcode of the request
     * @param[in]   bufferSize  the size of the buffer
    */
    HeaderRequest(char magic, short revision, OpCode opcode, unsigned bufferSize);

    /*! \brief Construct the request with a buffer.
     * Construct all the response structure with a buffer, if a parameter is missing, then
     * the parameter is set to default value
     * @param[in]   buffer          The buffer used to construct the structure
     * @param[in]   buffer_length   Then length of the buffer
    */
    HeaderRequest(const char *buffer, std::size_t buffer_length);

    /*! \brief Set all the datas in the body separate by a \0
     * @param[out]   dest    The destination of the data, the data function is not responsible of the
     * allocation of dest, you have to make sure that the dest is correctly set.
    */
    void data(char *dest) const;

    /*! \brief Return the size of the header */
    std::size_t size() const;
private:
    void fill_revision(std::size_t &idx, const char *buffer, std::size_t buffer_length);
    void fill_op_code(std::size_t &idx, const char *buffer, std::size_t buffer_length);
    void fill_buffer_size(std::size_t &idx, const char *buffer, std::size_t buffer_length);
};

/*! \brief This structure contains the header and the body of the request */
struct Request {
    HeaderRequest header; /*!< Header of the request */
    Body body; /*!< Body of the request */

    /*! \brief Construct the request with the default value of the header and the body */
    Request(char magic = HeaderUtility::default_magic_,
            short revision = HeaderUtility::default_rev_,
            OpCode opCode = OpCode::CLIENT_DISCONNECT, unsigned bufferSize = HeaderUtility::default_buffer_size_) noexcept;

    /*! \brief Construct the request with the header and the body
     * @param[in]       magic       The magic number of the request
     * @param[in]       revision    The revision of the request
     * @param[in]       opCode      The opCode of the request
     * @param[in]       bufferSize  The size of the body
     * @param[in]       body        The content of the body
    */
    Request(char magic, short revision, OpCode opCode, unsigned bufferSize, std::initializer_list<std::string> body);

    /*! \brief Construct a request structure with an array of bytes
     * @param[in]   buffer          The array of bytes containing the request
     * @param[in]   buffer_length   The length of the buffer
    */
    Request(const char *buffer, std::size_t buffer_length);

    /*! \brief Generate a sequence of bytes containing the header and the body of a request
     * The Request structure is not responsible of the allocation of dest parameter
     * @param[out]   dest    the destination byte array.
    */
    void data(char *dest) const;

    /*! \brief return the size of the request byte array */
    std::size_t size() const noexcept;
};

/*! \brief This structure contains the header and the body of the response */
struct Response {
    HeaderResponse header; /*!< Header of the response */
    Body body; /*!< Body of the response */

    /*! \brief Construct the request with the default value of the header and the body */
    Response(char magic = HeaderUtility::default_magic_,
            short revision = HeaderUtility::default_rev_,
            StatusCode statusCode = StatusCode::OK, unsigned bufferSize = HeaderUtility::default_buffer_size_) noexcept;

    Response(const HeaderResponse &h, const Body &b) : header{ h }, body{ b } {};

    /*! \brief Construct the request with the header and the body
     * @param[in]       magic       The magic number of the request
     * @param[in]       revision    The revision of the request
     * @param[in]       statusCode  The opCode of the request
     * @param[in]       bufferSize  The size of the body
     * @param[in]       body        The content of the body
    */
    Response(char magic, short revision, StatusCode statusCode, unsigned bufferSize, std::initializer_list<std::string> body);

    /*! \brief Construct a request structure with an array of bytes
     * @param[in]   buffer          The array of bytes containing the request
     * @param[in]   buffer_length   The length of the buffer
    */
    Response(const char *buffer, std::size_t buffer_length);

    /*! \brief Generate a sequence of bytes containing the header and the body of a request
     * The Request structure is not responsible of the allocation of dest parameter
     * @param[out]   dest    the destination byte array.
    */
    void data(char *dest) const;

    /*! \brief return the size of the request byte array */
    std::size_t size() const noexcept;
};

} // namespace protocol

#endif //SPACE_BATTLE_NETIOBUSINESS_HPP
