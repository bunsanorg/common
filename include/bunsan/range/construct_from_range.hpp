#pragma once

namespace bunsan{namespace range
{
    namespace traits
    {
        template <typename Container>
        struct range_constructor
        {
            template <typename Range>
            static Container call(const Range &range)
            {
                return Container(range.begin(), range.end());
            }
        };
    }

    template <typename Container, typename Range>
    inline Container construct_from_range(const Range &range)
    {
        return traits::range_constructor<Container>::call(range);
    }
}}
