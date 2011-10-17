#include "bunsan/util.hpp"

#include <cassert>

int main()
{
	boost::property_tree::ptree pt, vl;
	vl.put_value("231dasmnw");
	bunsan::ptree_get(pt, "some.path", vl)==vl;
	pt.get_child("some.path")==vl;
}

