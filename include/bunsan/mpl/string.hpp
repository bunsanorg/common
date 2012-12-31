#pragma once

#include <string>

#include <boost/mpl/string.hpp>

namespace bunsan{namespace mpl
{
    template <char ... Args>
    class string;

    namespace detail
    {
        template <char ... Args>
        struct at;

        template <char Head, char ... Args>
        struct at<Head, Args...>
        {
            static constexpr char call(const std::size_t i)
            {
                return i ? at<Args...>::call(i - 1) : Head;
            }
        };

        template <>
        struct at<>
        {
            static constexpr char call(const std::size_t)
            {
                return '\0';
            }
        };

        template <typename First, typename Second>
        struct append;

        template <char ... Args1, char ... Args2>
        struct append<string<Args1...>, string<Args2...>>
        {
            typedef string<Args1..., Args2...> type;
        };
    }

    template <char ... Args>
    class string
    {
    public:
        static constexpr std::size_t size = sizeof...(Args);

        static constexpr char at(const std::size_t i)
        {
            return detail::at<Args...>::call(i);
        }

        static const std::string std_string;
        static const char c_str[size + 1];
    };

    template <char ... Args>
    const std::string string<Args...>::std_string = {Args...};

    template <char ... Args>
    const char string<Args...>::c_str[string<Args...>::size + 1] = {Args..., '\0'};

    struct append
    {
        template <typename First, typename Second>
        struct apply: detail::append<First, Second> {};
    };
}}
