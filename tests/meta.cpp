#include <string>

#include "bunsan/meta/comparable.hpp"

namespace
{
    struct stub
    {
    };
}

int main()
{
    using namespace bunsan;
    static_assert(meta::is_less_than_comparable<int>::value, "is_less_than_comparable");
    static_assert(!meta::is_equality_comparable<stub>::value, "is_equality_comparable");
    static_assert(meta::is_greater_than_comparable<std::string>::value, "is_greater_than_comparable");
}
