#pragma once

#include "bunsan/tests/error.hpp"

#include <cstdlib>

#include <boost/filesystem/path.hpp>

namespace bunsan{namespace tests{namespace dir
{
    struct unset_environment_error: virtual error
    {
        typedef boost::error_info<struct tag_key, std::string> key;
    };

    namespace detail
    {
        const char *getenv(const char *const key)
        {
            BOOST_ASSERT(key);
            const char *value = ::getenv(key);
            if (!value)
                BOOST_THROW_EXCEPTION(unset_environment_error() << unset_environment_error::key(key));
            return value;
        }
    }

    static const boost::filesystem::path source = detail::getenv("BUNSAN_SOURCE_DIR");
    static const boost::filesystem::path binary = detail::getenv("BUNSAN_BINARY_DIR");

    namespace tests
    {
        static const boost::filesystem::path source = dir::source / "tests";
        static const boost::filesystem::path binary = dir::binary / "tests";

        namespace resources
        {
            static const boost::filesystem::path source = tests::source / "resources";
            static const boost::filesystem::path binary = tests::binary / "resources";
        }
    }
}}}
