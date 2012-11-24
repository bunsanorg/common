#define BOOST_TEST_MODULE error
#include <boost/test/unit_test.hpp>

#include "bunsan/error.hpp"
#include "bunsan/process/error.hpp"

BOOST_AUTO_TEST_SUITE(error)

template <typename Error>
bool check(const Error &e)
{
    BOOST_CHECK(e.template get<bunsan::error::stacktrace>());
    BOOST_TEST_MESSAGE(e.what());
    return true;
}

#define BUNSAN_CHECK_EXCEPTION(ERROR, ...) BOOST_CHECK_EXCEPTION(BOOST_THROW_EXCEPTION(ERROR(__VA_ARGS__)), ERROR, check<ERROR>)

BOOST_AUTO_TEST_CASE(throw_)
{
    BUNSAN_CHECK_EXCEPTION(bunsan::error, "message");
    BUNSAN_CHECK_EXCEPTION(bunsan::process::error, "message");
    BUNSAN_CHECK_EXCEPTION(bunsan::process::non_zero_exit_status_error, 10);
}

BOOST_AUTO_TEST_SUITE_END() // error
