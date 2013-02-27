#pragma once

namespace bunsan{namespace meta
{
    namespace detail
    {
        template <typename Self>
        struct back_impl;
    }

    template <typename Self>
    struct back: detail::back_impl<Self>::apply {};
}}
