#include "bunsan/util.hpp"
#include "bunsan/process/context.hpp"

#include <cassert>

int main()
{
	try
	{
		bunsan::process::context ctx;
		ctx.use_path();
	}
	catch (std::invalid_argument &e)
	{
		SLOG("caught: "<<e.what());
	}
}

