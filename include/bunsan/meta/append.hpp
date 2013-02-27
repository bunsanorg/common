#pragma once

#include "bunsan/meta/size.hpp"
#include "bunsan/meta/front.hpp"
#include "bunsan/meta/pop_front.hpp"
#include "bunsan/meta/push_back.hpp"

namespace bunsan{namespace meta
{
    namespace detail
    {
        template <typename Self>
        struct append_impl;
    }

    template <typename Self, typename Other>
    struct append: detail::append_impl<Self>::template apply<Other> {};

    namespace detail
    {
        namespace default_
        {
            template <typename Self, typename Other, typename Enable=void>
            struct append;

            template <typename Self, typename Other>
            struct append<Self, Other, typename std::enable_if<meta::empty<Other>::value, void>::type>
            {
                typedef Self type;
            };

            template <typename Self, typename Other>
            struct append<Self, Other, typename std::enable_if<!meta::empty<Other>::value, void>::type>
            {
                typedef typename append<
                    typename meta::push_back<Self, typename meta::front<Other>::type>::type,
                    typename meta::pop_front<Other>::type>::type type;
            };
        }

        template <typename Self>
        struct append_impl
        {
            template <typename Other>
            struct apply: default_::append<Self, Other> {};
        };
    }
}}
