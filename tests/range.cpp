#define BOOST_TEST_MODULE range
#include <boost/test/unit_test.hpp>

#include <bunsan/range/construct_from_range.hpp>

#include <set>
#include <vector>

BOOST_AUTO_TEST_SUITE(range)

BOOST_AUTO_TEST_CASE(construct_from_range)
{
    const std::vector<int> x = {1, 2, 3, 4, 5};
    const std::set<int> actual = bunsan::range::construct_from_range<std::set<int>>(x);
    const std::set<int> expected(x.begin(), x.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(actual.begin(), actual.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END() // range
