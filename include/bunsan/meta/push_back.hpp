#pragma once

namespace bunsan{namespace meta
{
    namespace detail
    {
        template <typename Self>
        struct push_back_impl;
    }

    template <typename Self, typename Item>
    struct push_back: detail::push_back_impl<Self>::template apply<Item> {};
}}
