#define BOOST_TEST_MODULE meta
#include <boost/test/unit_test.hpp>

#include <bunsan/meta/comparable.hpp>
#include <bunsan/meta/vector.hpp>

#include <string>
#include <type_traits>

BOOST_AUTO_TEST_SUITE(meta)

BOOST_AUTO_TEST_SUITE(containers)

BOOST_AUTO_TEST_CASE(vector)
{
    typedef bunsan::meta::vector<> e;
    typedef bunsan::meta::vector<int, float, char> a;
    typedef bunsan::meta::vector<double, unsigned> b;

    // size
    static_assert(bunsan::meta::size<e>::value == 0, "size == 0");
    static_assert(bunsan::meta::size<a>::value == 3, "size == 3");
    static_assert(bunsan::meta::size<b>::value == 2, "size == 2");

    // empty
    static_assert(bunsan::meta::empty<e>::value, "empty");
    static_assert(!bunsan::meta::empty<a>::value, "!empty");

    // clear
    static_assert(std::is_same<typename bunsan::meta::clear<a>::type, e>::value, "clear");

    // at
    static_assert(std::is_same<typename bunsan::meta::at<a, 1>::type, float>::value, "at");

    // front
    static_assert(std::is_same<typename bunsan::meta::front<a>::type, int>::value, "front");

    // push_front
    static_assert(std::is_same<typename bunsan::meta::pop_front<a>::type, bunsan::meta::vector<float, char>>::value, "push_front");

    // back
    static_assert(std::is_same<typename bunsan::meta::back<a>::type, char>::value, "back");

    // push_back
    static_assert(std::is_same<typename bunsan::meta::pop_back<a>::type, bunsan::meta::vector<int, float>>::value, "push_back");

    // append
    typedef typename bunsan::meta::append<a, b>::type c;
    static_assert(std::is_same<c, bunsan::meta::vector<int, float, char, double, unsigned>>::value, "append");

    // head
    typedef typename bunsan::meta::head<a, 2>::type d;
    static_assert(std::is_same<d, bunsan::meta::vector<int, float>>::value, "head");

    // skip_head
    typedef typename bunsan::meta::skip_head<a, 2>::type f;
    static_assert(std::is_same<f, bunsan::meta::vector<char>>::value, "skip_head");

    // tail
    typedef typename bunsan::meta::tail<a, 2>::type g;
    static_assert(std::is_same<g, bunsan::meta::vector<float, char>>::value, "tail");

    // skip_tail
    typedef typename bunsan::meta::skip_tail<a, 2>::type h;
    static_assert(std::is_same<h, bunsan::meta::vector<int>>::value, "skip_tail");
}

BOOST_AUTO_TEST_SUITE_END() // containers

struct stub {};

BOOST_AUTO_TEST_CASE(comparable)
{
    static_assert(bunsan::meta::is_less_than_comparable<int>::value, "is_less_than_comparable");
    static_assert(!bunsan::meta::is_equality_comparable<stub>::value, "is_equality_comparable");
    static_assert(bunsan::meta::is_greater_than_comparable<std::string>::value, "is_greater_than_comparable");
}

BOOST_AUTO_TEST_SUITE_END() // meta
