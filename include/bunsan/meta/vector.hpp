#pragma once

#include <type_traits>

#include <bunsan/meta/size.hpp>
#include <bunsan/meta/clear.hpp>
#include <bunsan/meta/at.hpp>
#include <bunsan/meta/append.hpp>
#include <bunsan/meta/front.hpp>
#include <bunsan/meta/back.hpp>
#include <bunsan/meta/push_front.hpp>
#include <bunsan/meta/push_back.hpp>
#include <bunsan/meta/pop_front.hpp>
#include <bunsan/meta/pop_back.hpp>
#include <bunsan/meta/head.hpp>
#include <bunsan/meta/tail.hpp>

namespace bunsan{namespace meta
{
    template <typename ... Args>
    struct vector {};

    namespace detail
    {
        namespace vector_
        {
            template <typename Arg, typename ... Args>
            struct split_front
            {
                typedef Arg front_type;
                typedef vector<Args...> tail_type;
            };

            template <typename Prefix, typename Current, typename ... Left>
            struct split_back_:
                split_back_<
                    typename meta::push_back<Prefix, Current>::type,
                    Left...> {};

            template <typename Prefix, typename Current>
            struct split_back_<Prefix, Current>
            {
                typedef Prefix head_type;
                typedef Current back_type;
            };

            template <typename ... Args>
            struct split_back: split_back_<vector<>, Args...> {};
        }

        template <typename ... Args>
        struct size_impl<vector<Args...>>
        {
            struct apply: std::integral_constant<std::size_t, sizeof...(Args)> {};
        };

        template <typename ... Args>
        struct clear_impl<vector<Args...>>
        {
            struct apply
            {
                typedef vector<> type;
            };
        };

        template <typename ... Args>
        struct front_impl<vector<Args...>>
        {
            struct apply
            {
                typedef typename vector_::split_front<Args...>::front_type type;
            };
        };

        template <typename ... Args>
        struct back_impl<vector<Args...>>
        {
            struct apply
            {
                typedef typename vector_::split_back<Args...>::back_type type;
            };
        };

        template <typename ... Args>
        struct push_front_impl<vector<Args...>>
        {
            template <typename Item>
            struct apply
            {
                typedef vector<Item, Args...> type;
            };
        };

        template <typename ... Args>
        struct push_back_impl<vector<Args...>>
        {
            template <typename Item>
            struct apply
            {
                typedef vector<Args..., Item> type;
            };
        };

        template <typename ... Args>
        struct pop_front_impl<vector<Args...>>
        {
            struct apply
            {
                typedef typename vector_::split_front<Args...>::tail_type type;
            };
        };

        template <typename ... Args>
        struct pop_back_impl<vector<Args...>>
        {
            struct apply
            {
                typedef typename vector_::split_back<Args...>::head_type type;
            };
        };
    }
}}
