#pragma once

#include <bunsan/logging/keywords/scope.hpp>
#include <bunsan/logging/sources/scope.hpp>

#include <boost/current_function.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/trivial.hpp>

namespace bunsan{namespace logging{namespace trivial
{
    template <typename Level = int>
    class logger: public boost::log::sources::basic_composite_logger<
        char,
        logger<Level>,
        boost::log::sources::multi_thread_model<
            boost::log::aux::light_rw_mutex
        >,
        boost::log::sources::features<
            boost::log::sources::severity<Level>,
            sources::scope
        >
    > {};

    // FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME
    BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(
        global,
        logger<boost::log::trivial::severity_level>
    )
}}}

#define BUNSAN_LOG_TRIVIAL(SEV) \
    BOOST_LOG_STREAM_WITH_PARAMS( \
        ::bunsan::logging::trivial::global::get(), \
        (::boost::log::keywords::severity = ::boost::log::trivial::SEV) \
        (::bunsan::logging::keywords::file = __FILE__) \
        (::bunsan::logging::keywords::line = __LINE__) \
        (::bunsan::logging::keywords::function = __func__) \
        (::bunsan::logging::keywords::pretty_function = BOOST_CURRENT_FUNCTION) \
    )

#define BUNSAN_LOG_TRACE BUNSAN_LOG_TRIVIAL(trace)
#define BUNSAN_LOG_DEBUG BUNSAN_LOG_TRIVIAL(debug)
#define BUNSAN_LOG_INFO BUNSAN_LOG_TRIVIAL(info)
#define BUNSAN_LOG_WARNING BUNSAN_LOG_TRIVIAL(warning)
#define BUNSAN_LOG_ERROR BUNSAN_LOG_TRIVIAL(error)
#define BUNSAN_LOG_FATAL BUNSAN_LOG_TRIVIAL(fatal)
