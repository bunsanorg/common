#define BOOST_TEST_MODULE meta
#include <boost/test/unit_test.hpp>

#include <string>

#include "bunsan/meta/comparable.hpp"

BOOST_AUTO_TEST_SUITE(meta)

struct stub
{
};

BOOST_AUTO_TEST_CASE(comparable)
{
    static_assert(bunsan::meta::is_less_than_comparable<int>::value, "is_less_than_comparable");
    static_assert(!bunsan::meta::is_equality_comparable<stub>::value, "is_equality_comparable");
    static_assert(bunsan::meta::is_greater_than_comparable<std::string>::value, "is_greater_than_comparable");
}

BOOST_AUTO_TEST_SUITE_END() // meta
