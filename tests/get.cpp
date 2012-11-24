#include "bunsan/get.hpp"
#include "bunsan/util.hpp"

#include <string>

#include <cassert>
#include <cstring>

#define test_m \
    try \
    { \
        bunsan::get(o, msg); \
        assert(o); \
        SLOG("optional is initialized"); \
    } \
    catch (std::invalid_argument &e) \
    { \
        assert(!o); \
        assert(msg==e.what()); \
        SLOG("caught: "<<e.what()); \
    }

#define test_e_ \
    try \
    { \
        bunsan::get(o, msg); \
        assert(o); \
        SLOG("optional is initialized"); \
    } \
    catch (E &e) \

#define test_ee test_e_ \
    { \
        assert(!o); \
        assert(e==msg); \
        SLOG("caught: "<<e); \
    }

#define test_se test_e_ \
    { \
        assert(!o); \
        assert(!strcmp(e.what(), msg.what())); \
        SLOG("caught: "<<e.what()); \
    }

template <typename T>
void test_msg(boost::optional<T> &o, const std::string &msg)
{
    test_m
}

template <typename T>
void test_msg(const boost::optional<T> &o, const std::string &msg)
{
    test_m
}

template <typename T, typename E>
void test_sexcept(boost::optional<T> &o, const E &msg)
{
    test_se
}

template <typename T, typename E>
void test_sexcept(const boost::optional<T> &o, const E &msg)
{
    test_se
}

template <typename T, typename E>
void test_except(boost::optional<T> &o, const E &msg)
{
    test_ee
}

template <typename T, typename E>
void test_except(const boost::optional<T> &o, const E &msg)
{
    test_ee
}

int main()
{
    boost::optional<int> o;
    test_msg(o, "adsdkmas wkdmnakjndjkasnjd");
    test_except(o, 100500);
    test_sexcept(o, std::logic_error("jdknasjkdnc amk mkamnaklm"));
    o = 10;
    test_msg(o, "dnjasndkasndjknasjkdn");
    test_except(o, 100500);
    test_sexcept(o, std::logic_error("jdknasjkdnc amk mkamnaklm"));
}
