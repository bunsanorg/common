#include "bunsan/factory.hpp"

#include <memory>
#include <set>

#include <boost/assert.hpp>

int main()
{
    typedef bunsan::factory<std::shared_ptr<int>()> bunsan_factory;
    typename bunsan_factory::map_type *map(0);
    std::set<bunsan_factory::key_type> set(
        bunsan_factory::registered_begin(map),
        bunsan_factory::registered_end(map));
    BOOST_ASSERT(set.empty());
}
