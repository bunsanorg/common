#pragma once

#include "bunsan/meta/front.hpp"
#include "bunsan/meta/push_front.hpp"
#include "bunsan/meta/pop_front.hpp"

namespace bunsan{namespace meta
{
    namespace detail
    {
        template <typename Self>
        struct head_impl;

        template <typename Self>
        struct skip_head_impl;
    }

    template <typename Self, std::size_t size>
    struct head: detail::head_impl<Self>::template apply<size> {};

    template <typename Self, std::size_t size>
    struct skip_head: detail::skip_head_impl<Self>::template apply<size> {};

    namespace detail
    {
        namespace default_
        {
            template <std::size_t size>
            struct head
            {
                template <typename Self>
                struct apply:
                    meta::push_front<
                        typename head<size - 1>::template apply<typename meta::pop_front<Self>::type>::type,
                        typename meta::front<Self>::type> {};
            };

            template <>
            struct head<0>
            {
                template <typename Self>
                struct apply: meta::clear<Self> {};
            };

            template <std::size_t size>
            struct skip_head
            {
                template <typename Self>
                struct apply: skip_head<size - 1>::template apply<typename meta::pop_front<Self>::type> {};
            };

            template <>
            struct skip_head<0>
            {
                template <typename Self>
                struct apply
                {
                    typedef Self type;
                };
            };
        }

        template <typename Self>
        struct head_impl
        {
            template <std::size_t size>
            struct apply: default_::head<size>::template apply<Self> {};
        };

        template <typename Self>
        struct skip_head_impl
        {
            template <std::size_t size>
            struct apply: default_::skip_head<size>::template apply<Self> {};
        };
    }
}}
