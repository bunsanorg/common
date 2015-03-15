#include <bunsan/logging/core.hpp>

namespace bunsan{namespace logging
{
    bool set_enabled(bool enabled)
    {
        return boost::log::core::get()->set_logging_enabled(enabled);
    }
}}
