#include <memory>

#include <cassert>

#include "factory.hpp"

int main()
{
	using namespace bunsan::factory;
	typedef std::shared_ptr<int> int_ptr;
	std::map<std::string, std::function<int_ptr()>> *map(0);
	assert(!static_cast<bool>(instance(map, "strange")));
	assert(  register_new(map, "zero", std::function<int_ptr()>( [](){return int_ptr(new int(0));} ))  );
	assert(map!=0);
	assert(!  register_new(map, "zero", std::function<int_ptr()>( [](){return int_ptr(new int(-1));} ))  );
	assert(*instance(map, "zero")==0);
	assert(  register_new(map, "one", std::function<int_ptr()>( [](){return int_ptr(new int(1));} ))  );
	assert(*instance(map, "zero")==0);
	assert(*instance(map, "one")==1);
	assert(!static_cast<bool>(instance(map, "")));
	return 0;
}

