#include <memory>
#include <set>

#include <cassert>

#include "bunsan/factory.hpp"

int main()
{
    typedef std::shared_ptr<int> int_ptr;
    typedef bunsan::factory<int_ptr()> bunsan_factory;
    typename bunsan_factory::map_type *map(0);
    assert(!static_cast<bool>(bunsan_factory::instance(map, "strange")));
    assert(  bunsan_factory::register_new(map, "zero", [](){return int_ptr(new int(0));} )  );
    assert(map!=0);
    assert(!  bunsan_factory::register_new(map, "zero", [](){return int_ptr(new int(-1));} )  );
    assert(*bunsan_factory::instance(map, "zero")==0);
    assert(  bunsan_factory::register_new(map, "one", [](){return int_ptr(new int(1));} )  );
    assert(*bunsan_factory::instance(map, "zero")==0);
    assert(*bunsan_factory::instance(map, "one")==1);
    assert(!static_cast<bool>(bunsan_factory::instance(map, "")));
    {
        std::set<std::string> set(bunsan_factory::registered_begin(map), bunsan_factory::registered_end(map));
        std::set<std::string> req = {"zero", "one"};
        assert(req==set);
    }
    return 0;
}

