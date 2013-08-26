#pragma once

#include <bunsan/none.hpp>

#include <boost/optional.hpp>

#include <type_traits>

namespace bunsan
{
    static_assert(std::is_same<none_t, boost::none_t>::value, "Should be the same.");

    template <typename T>
    class optional: public boost::optional<T>
    {
    private:
        typedef boost::optional<T> base_type;

    public:
        using base_type::base_type;

        optional()=default;

        optional(const optional &)=default;
        optional &operator=(const optional &)=default;

        optional(optional &&o) noexcept
        {
            swap(o);
            o = none;
        }

        optional &operator=(optional &&o) noexcept
        {
            swap(o);
            o.reset();
            return *this;
        }

        inline void swap(optional<T> &o) noexcept
        {
            base_type::swap(o);
        }
    };

    template <typename T>
    inline void swap(optional<T> &a, optional<T> &b) noexcept
    {
        a.swap(b);
    }
}
