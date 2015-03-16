#include <bunsan/logging/core.hpp>

#include <bunsan/logging/trivial.hpp>

namespace bunsan{namespace logging
{
    bool set_enabled(bool enabled)
    {
        return boost::log::core::get()->set_logging_enabled(enabled);
    }

    void remove_default_sink()
    {
        trivial::global::remove_default_sink();
    }
}}
