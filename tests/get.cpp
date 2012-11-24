#define BOOST_TEST_MODULE get
#include <boost/test/unit_test.hpp>

#include "bunsan/get.hpp"

#define BUNSAN_CHECK_EXCEPTION(OPT, MSG) BOOST_CHECK_EXCEPTION(bunsan::get(OPT, MSG), bunsan::uninitialized_optional_error, \
    [](const bunsan::uninitialized_optional_error &e) \
    { \
        return *e.get<bunsan::error::message>() == MSG; \
    })

BOOST_AUTO_TEST_SUITE(get)

BOOST_AUTO_TEST_CASE(optional)
{
    boost::optional<int> o;
    BUNSAN_CHECK_EXCEPTION(o, "adsdkmas wkdmnakjndjkasnjd");
    o = 10;
    BOOST_CHECK_EQUAL(bunsan::get(o, "dnjasndkasndjknasjkdn"), 10);
}

BOOST_AUTO_TEST_SUITE_END() // get
