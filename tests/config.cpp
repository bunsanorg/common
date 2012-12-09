#define BOOST_TEST_MODULE config
#include <boost/test/unit_test.hpp>

#include "bunsan/config/traits.hpp"
#include "bunsan/config/input_archive.hpp"

#include "bunsan/stream_enum.hpp"

#include <boost/property_tree/ptree.hpp>

struct parent {};
struct derived {};

BUNSAN_CONFIG_EXPORT(::parent, ::derived, "hello, world!")

typedef boost::variant<std::string, int> variant;

BUNSAN_CONFIG_EXPORT(variant, std::string, "string")
BUNSAN_CONFIG_EXPORT(variant, int, "int")

BOOST_AUTO_TEST_SUITE(config)

BOOST_AUTO_TEST_CASE(traits)
{
    enum my_enum {};
    static_assert(bunsan::config::traits::type_key<struct a, struct b>::call() == nullptr, "not defined");
    static_assert(bunsan::config::traits::type_key< ::parent, ::derived>::call(), "defined");
    static_assert(bunsan::config::traits::direct_assign<std::string>::value, "direct");
    static_assert(bunsan::config::traits::direct_assign<int>::value, "direct");
    static_assert(bunsan::config::traits::direct_assign<my_enum>::value, "direct");
    BOOST_CHECK_EQUAL((bunsan::config::traits::type_key< ::parent, ::derived>::call()), "hello, world!");
}

struct fixture
{
    struct cfg_type
    {

        BUNSAN_INCLASS_STREAM_ENUM_CLASS(my_enum,
        (
            first,
            second
        ))

        int a;
        std::string b;
        my_enum c;
        variant d;

        template <typename Archive>
        void serialize(Archive &ar, const unsigned int)
        {
            ar & BOOST_SERIALIZATION_NVP(a);
            ar & BOOST_SERIALIZATION_NVP(b);
            ar & BOOST_SERIALIZATION_NVP(c);
            ar & BOOST_SERIALIZATION_NVP(d);
        }
    };

    fixture()
    {
        tree_int.put("a", 10);
        tree_int.put("b", "hello");
        tree_int.put("c", "second");
        tree_multiple = tree_no = tree_other = tree_str = tree_int;
        tree_int.put("d.int", "20");
        tree_str.put("d.string", "string");
        tree_other.put("d.other", "something");
        tree_no.put("d", ""); // we need d present with no (empty) value and no children
        tree_multiple.put("d.int", 30);
        tree_multiple.put("d.string", "hello");
    }

    boost::property_tree::ptree tree_int, tree_str, tree_other, tree_no, tree_multiple;
};

BOOST_FIXTURE_TEST_SUITE(archive, fixture)

BOOST_AUTO_TEST_CASE(input)
{
    cfg_type cfg_int, cfg_str, cfg_err;
    bunsan::config::input_archive<boost::property_tree::ptree>
        in_int(tree_int), in_str(tree_str), in_other(tree_other), in_no(tree_no), in_multiple(tree_multiple);
    in_int >> cfg_int;
    in_str >> cfg_str;
    BOOST_CHECK_THROW(in_other >> cfg_err, bunsan::config::variant_load_invalid_key_error);
    BOOST_CHECK_THROW(in_no >> cfg_err, bunsan::config::variant_load_no_key_error);
    BOOST_CHECK_THROW(in_multiple >> cfg_err, bunsan::config::variant_load_multiple_keys_error);
    BOOST_CHECK_EQUAL(cfg_int.a, 10);
    BOOST_CHECK_EQUAL(cfg_int.b, "hello");
    BOOST_CHECK_EQUAL(cfg_int.c, cfg_type::my_enum::second);
    BOOST_CHECK(boost::get<int>(&cfg_int.d));
    if (boost::get<int>(&cfg_int.d))
        BOOST_CHECK_EQUAL(*boost::get<int>(&cfg_int.d), 20);
    BOOST_CHECK(boost::get<std::string>(&cfg_str.d));
    if (boost::get<std::string>(&cfg_str.d))
        BOOST_CHECK_EQUAL(*boost::get<std::string>(&cfg_str.d), "string");
}

BOOST_AUTO_TEST_SUITE_END() // archive

BOOST_AUTO_TEST_SUITE_END() // config
