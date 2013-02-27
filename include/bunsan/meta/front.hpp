#pragma once

namespace bunsan{namespace meta
{
    namespace detail
    {
        template <typename Self>
        struct front_impl;
    }

    template <typename Self>
    struct front: detail::front_impl<Self>::apply {};
}}
