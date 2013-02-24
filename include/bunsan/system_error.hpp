#pragma once

#include "bunsan/categorized_error.hpp"

namespace bunsan
{
    struct system_error: virtual categorized_error
    {
        system_error();
        explicit system_error(const int errcode);
        explicit system_error(const std::string &what);
        system_error(const int errcode, const std::string &what);

        // inherited
        explicit system_error(const std::error_code &ec);
        system_error(const std::error_code &ec, const std::string &what);
    };
}
