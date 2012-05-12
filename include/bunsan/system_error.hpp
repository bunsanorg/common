#ifndef BUNSAN_SYSTEM_ERROR_HPP
#define BUNSAN_SYSTEM_ERROR_HPP

#include <system_error>
#include <string>

namespace bunsan
{
    inline void system_error(const std::string &msg)
    {
        throw std::system_error(std::error_code(errno, std::system_category()), msg);
    }
}

#endif //BUNSAN_SYSTEM_ERROR_HPP

