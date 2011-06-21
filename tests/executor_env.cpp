#include <sstream>

#include <cassert>

#include <boost/property_tree/info_parser.hpp>

#include "bunsan/executor.hpp"

int main()
{
	using namespace boost::property_tree;
	using bunsan::executor;
	using bunsan::process::context;
	typedef std::stringstream ss;
	typedef std::vector<std::string> vs;
	ptree pt;
	context ctx;
	ss in_1("d\n"
		"{\n"
		"\texecutable exe\n"
		"\tcurrent_path path\n"
		"}\n");
	read_info(in_1, pt);
	ctx = executor(pt).context();
	assert(ctx.executable()=="exe");
	assert(ctx.current_path()=="path");
}

