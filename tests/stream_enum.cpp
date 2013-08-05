#define BOOST_TEST_MODULE stream_enum
#include <boost/test/unit_test.hpp>

#include <bunsan/stream_enum.hpp>

#include <boost/lexical_cast.hpp>

BOOST_AUTO_TEST_SUITE(stream_enum)

BOOST_AUTO_TEST_SUITE(free)

BOOST_AUTO_TEST_SUITE(class_)

BUNSAN_STREAM_ENUM_CLASS(my_enum,
(
    FIRST,
    SECOND,
    THIRD
))

BOOST_AUTO_TEST_CASE(test)
{
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(my_enum::FIRST), "FIRST");
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(my_enum::SECOND), "SECOND");
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(my_enum::THIRD), "THIRD");
    BOOST_CHECK_THROW(boost::lexical_cast<std::string>(static_cast<my_enum>(100500)),
                      my_enum_invalid_enum_representation_error);
    BOOST_CHECK(boost::lexical_cast<my_enum>("FIRST") == my_enum::FIRST);
    BOOST_CHECK(boost::lexical_cast<my_enum>("SECOND") == my_enum::SECOND);
    BOOST_CHECK(boost::lexical_cast<my_enum>("THIRD") == my_enum::THIRD);
    BOOST_CHECK_THROW(boost::lexical_cast<my_enum>("unknown"),
                      my_enum_invalid_string_representation_error);
}

BOOST_AUTO_TEST_SUITE_END() // class_

BOOST_AUTO_TEST_SUITE(old)

BUNSAN_STREAM_ENUM(my_enum,
(
    FIRST,
    SECOND,
    THIRD
))

BOOST_AUTO_TEST_CASE(test)
{
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(FIRST), "FIRST");
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(SECOND), "SECOND");
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(THIRD), "THIRD");
    BOOST_CHECK_THROW(boost::lexical_cast<std::string>(static_cast<my_enum>(100500)),
                      my_enum_invalid_enum_representation_error);
    BOOST_CHECK(boost::lexical_cast<my_enum>("FIRST") == FIRST);
    BOOST_CHECK(boost::lexical_cast<my_enum>("SECOND") == SECOND);
    BOOST_CHECK(boost::lexical_cast<my_enum>("THIRD") == THIRD);
    BOOST_CHECK_THROW(boost::lexical_cast<my_enum>("unknown"),
                      my_enum_invalid_string_representation_error);
}

BOOST_AUTO_TEST_SUITE_END() // old

BOOST_AUTO_TEST_SUITE_END() // free

BOOST_AUTO_TEST_SUITE(in_class)

BOOST_AUTO_TEST_SUITE(class_)

struct my_struct
{
    BUNSAN_INCLASS_STREAM_ENUM_CLASS(my_enum,
    (
        FIRST,
        SECOND,
        THIRD
    ))
};

BOOST_AUTO_TEST_CASE(test)
{
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(my_struct::my_enum::FIRST), "FIRST");
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(my_struct::my_enum::SECOND), "SECOND");
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(my_struct::my_enum::THIRD), "THIRD");
    BOOST_CHECK_THROW(boost::lexical_cast<std::string>(static_cast<my_struct::my_enum>(100500)),
                      my_struct::my_enum_invalid_enum_representation_error);
    BOOST_CHECK(boost::lexical_cast<my_struct::my_enum>("FIRST") == my_struct::my_enum::FIRST);
    BOOST_CHECK(boost::lexical_cast<my_struct::my_enum>("SECOND") == my_struct::my_enum::SECOND);
    BOOST_CHECK(boost::lexical_cast<my_struct::my_enum>("THIRD") == my_struct::my_enum::THIRD);
    BOOST_CHECK_THROW(boost::lexical_cast<my_struct::my_enum>("unknown"),
                      my_struct::my_enum_invalid_string_representation_error);
}

BOOST_AUTO_TEST_SUITE_END() // class_

BOOST_AUTO_TEST_SUITE(old)

struct my_struct
{
    BUNSAN_INCLASS_STREAM_ENUM(my_enum,
    (
        FIRST,
        SECOND,
        THIRD
    ))
};

BOOST_AUTO_TEST_CASE(test)
{
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(my_struct::FIRST), "FIRST");
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(my_struct::SECOND), "SECOND");
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(my_struct::THIRD), "THIRD");
    BOOST_CHECK_THROW(boost::lexical_cast<std::string>(static_cast<my_struct::my_enum>(100500)),
                      my_struct::my_enum_invalid_enum_representation_error);
    BOOST_CHECK(boost::lexical_cast<my_struct::my_enum>("FIRST") == my_struct::FIRST);
    BOOST_CHECK(boost::lexical_cast<my_struct::my_enum>("SECOND") == my_struct::SECOND);
    BOOST_CHECK(boost::lexical_cast<my_struct::my_enum>("THIRD") == my_struct::THIRD);
    BOOST_CHECK_THROW(boost::lexical_cast<my_struct::my_enum>("unknown"),
                      my_struct::my_enum_invalid_string_representation_error);
}

BOOST_AUTO_TEST_SUITE_END() // old

BOOST_AUTO_TEST_SUITE_END() // in_class

BOOST_AUTO_TEST_SUITE_END() // stream_enum
