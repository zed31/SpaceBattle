//
// Created by Clement on 11/16/2017.
//

#ifndef SPACE_BATTLE_NON_COPYABLE_HPP
#define SPACE_BATTLE_NON_COPYABLE_HPP

namespace space_battle {

/*! \brief class that remove the copy operators */
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable &copy) = delete;
    NonCopyable &operator=(const NonCopyable &copy) = delete;
};

} // namespace space_battle

#endif //SPACE_BATTLE_NON_COPYABLE_HPP
