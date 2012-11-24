#define BOOST_TEST_MODULE runtime
#include <boost/test/unit_test.hpp>

#include "bunsan/runtime/demangle.hpp"

struct X
{
    struct Y {};
    std::string f()
    {
    }
};

BOOST_AUTO_TEST_SUITE(runtime)

BOOST_AUTO_TEST_CASE(demangle)
{
    BOOST_CHECK_EQUAL(bunsan::runtime::type_name(X::Y()), "X::Y");
}

BOOST_AUTO_TEST_SUITE_END() // runtime
