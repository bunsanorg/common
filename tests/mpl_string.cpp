#define BOOST_TEST_MODULE mpl_string
#include <boost/test/unit_test.hpp>

#include "bunsan/mpl/string.hpp"

#include <type_traits>

BOOST_AUTO_TEST_SUITE(mpl_string)

BOOST_AUTO_TEST_CASE(boost_mpl_string)
{
    typedef boost::mpl::string<'1', '23', '456'> first;
    typedef boost::mpl::string<'1234', '56'> second;
    static_assert(!std::is_same<first, second>::value, "");
    BOOST_CHECK_EQUAL(boost::mpl::c_str<first>::value, boost::mpl::c_str<second>::value);
}

BOOST_AUTO_TEST_CASE(string)
{
    typedef bunsan::mpl::string<'1', '2', '3'> str;
    static_assert(str::size == 3, "");
    BOOST_CHECK_EQUAL(str::c_str, "123");
    BOOST_CHECK_EQUAL(str::std_string, "123");
}

BOOST_AUTO_TEST_CASE(append)
{
    typedef bunsan::mpl::string<'1', '2', '3'> first;
    typedef bunsan::mpl::string<'4', '5', '6'> second;
    typedef bunsan::mpl::append::apply<first, second>::type cat;
    static_assert(std::is_same<cat, bunsan::mpl::string<'1', '2', '3', '4', '5', '6'>>::value, "");
    BOOST_CHECK_EQUAL(cat::c_str, "123456");
}

BOOST_AUTO_TEST_SUITE_END() // mpl_string
