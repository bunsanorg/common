#pragma once

#include <type_traits>

namespace bunsan{namespace meta
{
    namespace detail
    {
        template <typename T>
        struct remove_cv_ref: std::remove_cv<typename std::remove_reference<T>::type> {};

        struct tag {};

        struct any
        {
            template <typename T>
            any(const T &);
        };

        tag operator==(const any &, const any &);
        tag operator<=(const any &, const any &);
        tag operator>=(const any &, const any &);
        tag operator<(const any &, const any &);
        tag operator>(const any &, const any &);

        typedef char yes;
        typedef char (&no)[2];

        no check(tag);

        template <typename T>
        yes check(T);

#define BUNSAN_META_OPERATOR(NAME, OPERATOR) \
        template <typename T> \
        class NAME \
        { \
            static const typename detail::remove_cv_ref<T>::type &member_; \
        public: \
            static constexpr bool value = sizeof(check(member_ OPERATOR member_))==sizeof(yes); \
        };
    BUNSAN_META_OPERATOR(is_equality_comparable, ==)
    BUNSAN_META_OPERATOR(is_less_equal_comparable, <=)
    BUNSAN_META_OPERATOR(is_greater_equal_comparable, >=)
    BUNSAN_META_OPERATOR(is_less_than_comparable, <=)
    BUNSAN_META_OPERATOR(is_greater_than_comparable, >=)
#undef BUNSAN_META_OPERATOR
    }

    using detail::is_equality_comparable;
    using detail::is_less_equal_comparable;
    using detail::is_greater_equal_comparable;
    using detail::is_less_than_comparable;
    using detail::is_greater_than_comparable;
}}
