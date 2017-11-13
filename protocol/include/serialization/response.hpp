//
// Created by 49844 on 12/11/2017.
//

#ifndef SPACE_BATTLE_RESPONSE_HPP
#define SPACE_BATTLE_RESPONSE_HPP

#include "data_utils.hpp"
#include "body.hpp"

namespace protocol {

namespace serialize {

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

} // namespace serialize

} // namespace protocol

#endif //SPACE_BATTLE_RESPONSE_HPP
