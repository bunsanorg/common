#pragma once

#include "bunsan/meta/back.hpp"
#include "bunsan/meta/head.hpp"

namespace bunsan{namespace meta
{
    namespace detail
    {
        template <typename Self>
        struct at_impl;
    }

    template <typename Self, std::size_t index>
    struct at: detail::at_impl<Self>::template apply<index> {};

    namespace detail
    {
        template <typename Self>
        struct at_impl
        {
            template <std::size_t index>
            struct apply: meta::back<typename meta::head<Self, index + 1>::type> {};
        };
    }
}}
