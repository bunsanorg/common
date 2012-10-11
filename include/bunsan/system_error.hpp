#ifndef BUNSAN_SYSTEM_ERROR_HPP
#define BUNSAN_SYSTEM_ERROR_HPP

#include "bunsan/error.hpp"

#include <system_error>

namespace bunsan
{
    struct system_error: virtual error
    {
        system_error();
        explicit system_error(const int errcode);
        explicit system_error(const std::error_code &ec);
        explicit system_error(const std::string &what);
        system_error(const int errcode, const std::string &what);
        system_error(const std::error_code &ec, const std::string &what);

        typedef boost::error_info<struct tag_error_code, std::error_code> error_code;
        typedef boost::error_info<struct tag_error_code_message, std::string> error_code_message;
        typedef boost::error_info<struct tag_what_message, std::string> what_message;
    };
}

#endif //BUNSAN_SYSTEM_ERROR_HPP

