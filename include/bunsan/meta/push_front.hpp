#pragma once

namespace bunsan{namespace meta
{
    namespace detail
    {
        template <typename Self>
        struct push_front_impl;
    }

    template <typename Self, typename Item>
    struct push_front: detail::push_front_impl<Self>::template apply<Item> {};
}}
