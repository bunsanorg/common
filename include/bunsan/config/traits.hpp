#pragma once

#include <string>
#include <type_traits>

namespace bunsan{namespace config{namespace traits
{
    /*!
     * \brief We assume that enum is declared using stream_enum.
     *
     * \todo Probably that should be checked (but for what?).
     */
    template <typename T>
    struct direct_assign:
        std::integral_constant<bool,
            std::is_arithmetic<T>::value || std::is_enum<T>::value> {};

    template <>
    struct direct_assign<std::string>: std::integral_constant<bool, true> {};

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
