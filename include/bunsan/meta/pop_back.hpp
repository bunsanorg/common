#pragma once

namespace bunsan{namespace meta
{
    namespace detail
    {
        template <typename Self>
        struct pop_back_impl;
    }

    template <typename Self>
    struct pop_back: detail::pop_back_impl<Self>::apply {};
}}
