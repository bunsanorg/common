#pragma once

#include <type_traits>

namespace bunsan{namespace meta
{
    namespace detail
    {
        template <typename Self>
        struct size_impl;

        template <typename Self>
        struct empty_impl;
    }

    template <typename Self>
    struct size: detail::size_impl<Self>::apply {};

    template <typename Self>
    struct empty: detail::empty_impl<Self>::apply {};

    namespace detail
    {
        template <typename Self>
        struct empty_impl
        {
            struct apply: std::integral_constant<bool, !size<Self>::apply::value> {};
        };
    }
}}
