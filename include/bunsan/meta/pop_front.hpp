#pragma once

namespace bunsan{namespace meta
{
    namespace detail
    {
        template <typename Self>
        struct pop_front_impl;
    }

    template <typename Self>
    struct pop_front: detail::pop_front_impl<Self>::apply {};
}}
