#pragma once

#include "bunsan/meta/back.hpp"
#include "bunsan/meta/push_back.hpp"
#include "bunsan/meta/pop_back.hpp"

namespace bunsan{namespace meta
{
    namespace detail
    {
        template <typename Self>
        struct tail_impl;

        template <typename Self>
        struct skip_tail_impl;
    }

    template <typename Self, std::size_t size>
    struct tail: detail::tail_impl<Self>::template apply<size> {};

    template <typename Self, std::size_t size>
    struct skip_tail: detail::skip_tail_impl<Self>::template apply<size> {};

    namespace detail
    {
        namespace default_
        {
            template <std::size_t size>
            struct tail
            {
                template <typename Self>
                struct apply:
                    meta::push_back<
                        typename tail<size - 1>::template apply<typename meta::pop_back<Self>::type>::type,
                        typename meta::back<Self>::type> {};
            };

            template <>
            struct tail<0>
            {
                template <typename Self>
                struct apply: meta::clear<Self> {};
            };

            template <std::size_t size>
            struct skip_tail
            {
                template <typename Self>
                struct apply: skip_tail<size - 1>::template apply<typename meta::pop_back<Self>::type> {};
            };

            template <>
            struct skip_tail<0>
            {
                template <typename Self>
                struct apply
                {
                    typedef Self type;
                };
            };
        }

        template <typename Self>
        struct tail_impl
        {
            template <std::size_t size>
            struct apply: default_::tail<size>::template apply<Self> {};
        };

        template <typename Self>
        struct skip_tail_impl
        {
            template <std::size_t size>
            struct apply: default_::skip_tail<size>::template apply<Self> {};
        };
    }
}}
