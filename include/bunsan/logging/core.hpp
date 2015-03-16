#pragma once

#include <boost/log/core.hpp>

namespace bunsan{namespace logging
{
    bool set_enabled(bool enabled);

    inline bool enable() { return set_enabled(true); }
    inline bool disable() { return set_enabled(false); }

    void remove_default_sink();
}}
