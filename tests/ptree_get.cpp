#define BOOST_TEST_MODULE ptree_get
#include <boost/test/unit_test.hpp>

#include "bunsan/util.hpp"

BOOST_AUTO_TEST_SUITE(ptree_get)

BOOST_AUTO_TEST_CASE(ptree_get)
{
    boost::property_tree::ptree pt, vl, vl2;
    vl.put_value("231dasmnw");
    vl2.put_value("njskdnjsdnjkasd");
    BOOST_CHECK(bunsan::ptree_get(pt, "some.path", vl) == vl);
    pt.get_child("some.path")==vl;
    BOOST_CHECK(bunsan::ptree_get(pt, "some.path", vl2) == vl);
    bunsan::ptree_get(pt, "some.path").put("name0", "value0");
    bunsan::ptree_get(pt, "some.path").put("name1", "value1");
    boost::property_tree::write_info(std::cout, pt);
    BOOST_CHECK_EQUAL(pt.get<std::string>("some.path.name0"), "value0");
    BOOST_CHECK_EQUAL(pt.get<std::string>("some.path.name1"), "value1");
}

BOOST_AUTO_TEST_SUITE_END() // ptree_get
