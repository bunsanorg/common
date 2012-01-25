#include "bunsan/error.hpp"
#include "bunsan/process/error.hpp"

#include <iostream>

#include <boost/assert.hpp>

template <typename Error>
struct helper
{
	template <typename Arg, typename Get>
	static void test(const Error &err, const Arg &arg, const Get &get)
	{
		try
		{
			BOOST_THROW_EXCEPTION(err);
			BOOST_ASSERT_MSG(false, "throw failed");
		}
		catch (Error &e)
		{
			BOOST_ASSERT_MSG(get(e)==arg, "construction failed");
			std::cerr<<e.what()<<std::endl;
			return;
		}
		catch (std::exception &e)
		{
			BOOST_ASSERT_MSG(false, "match failed");
		}
		BOOST_ASSERT_MSG(false, "exit failed");
	}
};

#define TEST(ERROR, ARG, GET) \
	helper<ERROR>::test(ERROR(ARG), ARG, [](const ERROR &e){return e.GET();})

int main()
{
	TEST(bunsan::error, std::string("message"), message);
	TEST(bunsan::process::error, std::string("message"), message);
	TEST(bunsan::process::non_zero_exit_status_error, 10, exit_status);
}

