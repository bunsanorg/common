#include "bunsan/error.hpp"

#include <iostream>

#include <boost/assert.hpp>

int main()
{
	try
	{
		BOOST_THROW_EXCEPTION(bunsan::error("message"));
		BOOST_ASSERT(false);
	}
	catch (bunsan::error &e)
	{
		BOOST_ASSERT(e.message()=="message");
		std::cerr<<e.what()<<std::endl;
		return 0;
	}
	catch (std::exception &e)
	{
		BOOST_ASSERT(false);
	}
	BOOST_ASSERT(false);
}

