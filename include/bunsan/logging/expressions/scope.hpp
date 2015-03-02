#pragma once

#include <bunsan/logging/severity.hpp>

#include <boost/log/expressions/keyword.hpp>

namespace bunsan{namespace logging{namespace expressions
{
    BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", logging::severity)
    BOOST_LOG_ATTRIBUTE_KEYWORD(file, "File", std::string)
    BOOST_LOG_ATTRIBUTE_KEYWORD(line, "Line", unsigned)
    BOOST_LOG_ATTRIBUTE_KEYWORD(function, "Function", std::string)
    BOOST_LOG_ATTRIBUTE_KEYWORD(pretty_function, "PrettyFunction", std::string)
}}}
