#pragma once

namespace bunsan{namespace meta
{
    namespace detail
    {
        template <typename Self>
        struct clear_impl;
    }

    template <typename Self>
    struct clear: detail::clear_impl<Self>::apply {};
}}
