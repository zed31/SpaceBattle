//
// Created by 49844 on 12/11/2017.
//

#ifndef SPACE_BATTLE_BODY_HPP
#define SPACE_BATTLE_BODY_HPP

#include <vector>
#include <string>

namespace protocol {

namespace serialize {

/*! \brief This class store the body and provide a way to transform it into char * */
class Body {
public:
    /*! \brief Default construction of the Body class */
    Body() = default;

    /*! \brief Construct the body with a set of data
     * @param[in] in    the initializer list used to generate the body
    */
    Body(std::initializer_list<std::string> in) noexcept;

    /*! \brief Construct the body with a set of data
     * @param[in] content    the vector used to generate the body
    */
    Body(const std::vector<std::string> &content) noexcept;

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

} // namespace serialize

} // namespace protocol

#endif //SPACE_BATTLE_BODY_HPP
