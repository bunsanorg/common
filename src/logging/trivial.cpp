#include <bunsan/logging/trivial.hpp>

#include <bunsan/logging/expressions/scope.hpp>

#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <iostream>
#include <mutex>

namespace bunsan{namespace logging{namespace trivial
{
    static void initialize()
    {
        namespace log = boost::log;
        namespace keywords = log::keywords;
        namespace attrs = log::attributes;
        namespace expr = log::expressions;

        log::add_common_attributes();
        log::add_console_log(
            std::cerr,
            keywords::format = (
                expr::stream << "[" <<
                expr::format_date_time<boost::posix_time::ptime>(
                    "TimeStamp",
                    "%d %b %Y %H:%M:%S.%f"
                ) <<
                " <" << logging::expressions::severity << "> " <<
                logging::expressions::file <<
                "(" << logging::expressions::function << ")" <<
                ":" << logging::expressions::line <<
                "] " <<
                expr::smessage
            )
        );
    }

    logger<severity> &global::get()
    {
        static logger<severity> log;
        static std::once_flag once;
        std::call_once(once, initialize);
        return log;
    }
}}}
