#define BOOST_TEST_MODULE tests
#include <boost/test/unit_test.hpp>

#include "bunsan/tests/environment.hpp"

#include <boost/filesystem/operations.hpp>

BOOST_AUTO_TEST_SUITE(tests)

BOOST_AUTO_TEST_CASE(detail_getenv)
{
    BOOST_CHECK_THROW(bunsan::tests::dir::detail::getenv("SOMETHING THAT WAS NOT SET"),
                      bunsan::tests::dir::unset_environment_error);
}

BOOST_AUTO_TEST_CASE(environment)
{
    BOOST_CHECK(boost::filesystem::exists(bunsan::tests::dir::source));
    BOOST_CHECK(boost::filesystem::exists(bunsan::tests::dir::binary));
    BOOST_CHECK(boost::filesystem::exists(bunsan::tests::dir::tests::source));
    BOOST_CHECK(boost::filesystem::exists(bunsan::tests::dir::tests::binary));
}

BOOST_AUTO_TEST_SUITE_END() // tests
