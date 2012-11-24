#define BOOST_TEST_MODULE factory
#include <boost/test/unit_test.hpp>

#include "bunsan/factory.hpp"

#include <memory>
#include <set>

BOOST_AUTO_TEST_SUITE(factory)

BOOST_AUTO_TEST_CASE(basic)
{
    typedef std::shared_ptr<int> int_ptr;
    typedef bunsan::factory<int_ptr()> bunsan_factory;
    typename bunsan_factory::map_type *map(0);
    BOOST_CHECK(!static_cast<bool>(bunsan_factory::instance(map, "strange")));
    BOOST_CHECK(  bunsan_factory::register_new(map, "zero", [](){return int_ptr(new int(0));} )  );
    BOOST_CHECK_NE(map, static_cast<typename bunsan_factory::map_type *>(nullptr));
    BOOST_CHECK(!  bunsan_factory::register_new(map, "zero", [](){return int_ptr(new int(-1));} )  );
    BOOST_CHECK_EQUAL(*bunsan_factory::instance(map, "zero"), 0);
    BOOST_CHECK(  bunsan_factory::register_new(map, "one", [](){return int_ptr(new int(1));} )  );
    BOOST_CHECK_EQUAL(*bunsan_factory::instance(map, "zero"), 0);
    BOOST_CHECK_EQUAL(*bunsan_factory::instance(map, "one"), 1);
    BOOST_CHECK(!static_cast<bool>(bunsan_factory::instance(map, "")));
    {
        std::set<std::string> set(bunsan_factory::registered_begin(map), bunsan_factory::registered_end(map));
        std::set<std::string> req = {"zero", "one"};
        BOOST_CHECK(req == set);
    }
}

BOOST_AUTO_TEST_CASE(null)
{
    typedef bunsan::factory<std::shared_ptr<int>()> bunsan_factory;
    typename bunsan_factory::map_type *map(0);
    std::set<bunsan_factory::key_type> set(
        bunsan_factory::registered_begin(map),
        bunsan_factory::registered_end(map));
    BOOST_CHECK(set.empty());
}

BOOST_AUTO_TEST_SUITE_END() // factory
