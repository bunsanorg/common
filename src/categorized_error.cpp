#include "bunsan/categorized_error.hpp"

namespace bunsan
{
    categorized_error::categorized_error(const std::error_code &ec)
    {
        (*this) << error_code(ec) << error_code_message(ec.message());
    }

    categorized_error::categorized_error(const std::error_code &ec, const std::string &what):
        categorized_error(ec)
    {
        (*this) << what_message(what);
    }
}
