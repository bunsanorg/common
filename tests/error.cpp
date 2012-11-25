#define BOOST_TEST_MODULE error
#include <boost/test/unit_test.hpp>

#include "bunsan/error.hpp"
#include "bunsan/enable_error_info.hpp"

#include "bunsan/filesystem/fstream.hpp"

BOOST_AUTO_TEST_SUITE(error)

struct
{
    template <typename Error>
    bool operator()(const Error &e) const
    {
        BOOST_TEST_MESSAGE(e.what());
        return true;
    }
} print;

struct
{
    bool operator()(const boost::exception &e) const
    {
        return boost::get_error_info<bunsan::error::stacktrace>(e);
    }
} check;


BOOST_AUTO_TEST_CASE(throw_)
{
    BOOST_CHECK_EXCEPTION(BOOST_THROW_EXCEPTION(bunsan::error("message")), bunsan::error, check);
}

// for stacktrace
template <typename Error, typename ... Args>
void throw_(Args &&...args)
{
    throw Error(std::forward<Args>(args)...);
}

void throw0()
{
    BUNSAN_EXCEPTIONS_WRAP_BEGIN()
    {
        throw_<std::runtime_error>("hello");
    }
    BUNSAN_EXCEPTIONS_WRAP_END()
}

void throw1()
{
    BUNSAN_EXCEPTIONS_WRAP_BEGIN()
    {
        throw_<bunsan::error>("world");
    }
    BUNSAN_EXCEPTIONS_WRAP_END()
}

void throw2()
{
    BUNSAN_EXCEPTIONS_WRAP_BEGIN()
    {
        bunsan::filesystem::ifstream fin("/definitely/not/existing/file/with/some/unpredictable/name");
        int x;
        fin >> x;
        fin.close();
    }
    BUNSAN_EXCEPTIONS_WRAP_END()
}

typedef boost::error_info<struct tag_info, std::string> info;
const std::string info_text = "INFO_TEXT_TO_CHECK";

bool check_info(const boost::exception &e)
{
    return check(e) && boost::get_error_info<info>(e) && *boost::get_error_info<info>(e) == info_text;
}

void throw_error_info()
{
    BUNSAN_EXCEPTIONS_WRAP_BEGIN()
    {
        throw_<std::runtime_error>("some text");
    }
    BUNSAN_EXCEPTIONS_WRAP_END_ERROR_INFO(info(info_text))
}

BOOST_AUTO_TEST_CASE(wrap)
{
    BOOST_CHECK_EXCEPTION(throw0(), std::runtime_error, print);
    BOOST_CHECK_EXCEPTION(throw0(), boost::exception, check);
    BOOST_CHECK_EXCEPTION(throw1(), bunsan::error, print);
    BOOST_CHECK_EXCEPTION(throw1(), bunsan::error, check);
    BOOST_CHECK_EXCEPTION(throw2(), std::exception, print);
    BOOST_CHECK_EXCEPTION(throw2(), boost::exception, check);
    BOOST_CHECK_EXCEPTION(throw_error_info(), std::exception, print);
    BOOST_CHECK_EXCEPTION(throw_error_info(), boost::exception, check_info);
}

BOOST_AUTO_TEST_SUITE_END() // error
