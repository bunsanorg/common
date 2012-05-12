#include "bunsan/util.hpp"

#include <cassert>

int main()
{
    boost::property_tree::ptree pt, vl, vl2;
    vl.put_value("231dasmnw");
    vl2.put_value("njskdnjsdnjkasd");
    assert(bunsan::ptree_get(pt, "some.path", vl)==vl);
    pt.get_child("some.path")==vl;
    assert(bunsan::ptree_get(pt, "some.path", vl2)==vl);
    bunsan::ptree_get(pt, "some.path").put("name0", "value0");
    bunsan::ptree_get(pt, "some.path").put("name1", "value1");
    boost::property_tree::write_info(std::cout, pt);
    assert(pt.get<std::string>("some.path.name0")=="value0");
    assert(pt.get<std::string>("some.path.name1")=="value1");
}

