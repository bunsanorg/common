#pragma once

#include <string>
#include <type_traits>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

namespace bunsan{namespace config{namespace traits
{
    /*!
     * \brief We assume that enum is declared using stream_enum.
     *
     * \todo Probably that should be checked (but for what?).
     */
    template <typename T>
    struct is_direct_assignable:
        std::integral_constant<bool,
            std::is_arithmetic<T>::value || std::is_enum<T>::value> {};

    template <typename T>
    struct is_random_access_sequence: std::integral_constant<bool, false> {};

    template <typename T>
    struct is_sequence: std::integral_constant<bool, false> {};

    template <typename T>
    struct is_set: std::integral_constant<bool, false> {};

    template <typename T>
    struct is_map: std::integral_constant<bool, false> {};

    /// Here we should exclude all selectors.
    template <typename T>
    struct is_recursive: std::integral_constant<bool,
        !is_direct_assignable<T>::value &&
        !is_random_access_sequence<T>::value &&
        !is_sequence<T>::value &&
        !is_set<T>::value &&
        !is_map<T>::value> {};

    template <typename Parent, typename Derived>
    struct type_key
    {
        /// \note it is not possible to define "const char *" field in header
        inline static constexpr const char *call()
        {
            return nullptr;
        }
    };
}}}

namespace bunsan{namespace config{namespace traits
{
    template <typename Char, typename Traits, typename Alloc>
    struct is_direct_assignable<std::basic_string<Char, Traits, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Tp, typename Alloc>
    struct is_random_access_sequence<std::vector<Tp, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Tp, typename Alloc>
    struct is_sequence<std::list<Tp, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Tp, typename Alloc>
    struct is_sequence<std::deque<Tp, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Key, typename Compare, typename Alloc>
    struct is_set<std::set<Key, Compare, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Key, typename Compare, typename Alloc>
    struct is_set<std::multiset<Key, Compare, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Key, typename Tp, typename Compare, typename Alloc>
    struct is_map<std::map<Key, Tp, Compare, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Key, typename Tp, typename Compare, typename Alloc>
    struct is_map<std::multimap<Key, Tp, Compare, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Value, typename Hash, typename Pred, typename Alloc>
    struct is_set<std::unordered_set<Value, Hash, Pred, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Value, typename Hash, typename Pred, typename Alloc>
    struct is_set<std::unordered_multiset<Value, Hash, Pred, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Key, typename Tp, typename Hash, typename Pred, typename Alloc>
    struct is_map<std::unordered_map<Key, Tp, Hash, Pred, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Key, typename Tp, typename Hash, typename Pred, typename Alloc>
    struct is_map<std::unordered_multimap<Key, Tp, Hash, Pred, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Value, typename Hash, typename Pred, typename Alloc>
    struct is_set<boost::unordered_set<Value, Hash, Pred, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Value, typename Hash, typename Pred, typename Alloc>
    struct is_set<boost::unordered_multiset<Value, Hash, Pred, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Key, typename Tp, typename Hash, typename Pred, typename Alloc>
    struct is_map<boost::unordered_map<Key, Tp, Hash, Pred, Alloc>>: std::integral_constant<bool, true> {};

    template <typename Key, typename Tp, typename Hash, typename Pred, typename Alloc>
    struct is_map<boost::unordered_multimap<Key, Tp, Hash, Pred, Alloc>>: std::integral_constant<bool, true> {};
}}}

#define BUNSAN_CONFIG_EXPORT(PARENT, DERIVED, FIELD) \
    namespace bunsan{namespace config{namespace traits \
    { \
        template <> \
        struct type_key<PARENT, DERIVED> \
        { \
            inline static constexpr const char *call() \
            { \
                return FIELD; \
            } \
        }; \
    }}}
