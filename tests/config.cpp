#define BOOST_TEST_MODULE config
#include <boost/test/unit_test.hpp>

#include "bunsan/config/traits.hpp"

struct parent {};
struct derived {};

BUNSAN_CONFIG_EXPORT(::parent, ::derived, "hello, world!");

BOOST_AUTO_TEST_SUITE(config)

BOOST_AUTO_TEST_CASE(traits)
{
    enum my_enum {};
    static_assert(bunsan::config::traits::type_key<struct a, struct b>::call() == nullptr, "not defined");
    static_assert(bunsan::config::traits::type_key< ::parent, ::derived>::call(), "defined");
    static_assert(bunsan::config::traits::direct_assign<std::string>::value, "direct");
    static_assert(bunsan::config::traits::direct_assign<int>::value, "direct");
    static_assert(bunsan::config::traits::direct_assign<my_enum>::value, "direct");
    BOOST_CHECK_EQUAL((bunsan::config::traits::type_key< ::parent, ::derived>::call()), "hello, world!");
}

BOOST_AUTO_TEST_SUITE_END() // config
