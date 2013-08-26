#define BOOST_TEST_MODULE optional
#include <boost/test/unit_test.hpp>

#include <bunsan/optional.hpp>

#include <memory>
#include <string>

BOOST_AUTO_TEST_SUITE(optional)

BOOST_AUTO_TEST_CASE(copy)
{
    bunsan::optional<std::string> default_ctor;
    bunsan::optional<std::string> a = std::string("123");
    bunsan::optional<std::string> b = a;
    BOOST_REQUIRE(a);
    BOOST_REQUIRE(b);
    BOOST_CHECK_EQUAL(*a, *b);
}

BOOST_AUTO_TEST_CASE(move)
{
    typedef std::unique_ptr<int> ptr_t;
    bunsan::optional<ptr_t> default_ctor;
    bunsan::optional<ptr_t> a(new int(10));
    bunsan::optional<ptr_t> b = std::move(a);
    BOOST_CHECK(!a);
    BOOST_REQUIRE(b);
    BOOST_REQUIRE(*b);
    BOOST_CHECK_EQUAL(**b, 10);
}

BOOST_AUTO_TEST_SUITE_END() // optional
