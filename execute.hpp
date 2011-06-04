#ifndef IMPL_EXECUTE_HPP
#define IMPL_EXECUTE_HPP

#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>

#include "process.hpp"

namespace bunsan
{
	int sync_execute(const boost::filesystem::path &cwd, const std::vector<std::string> &args);
	process_ptr async_execute(const boost::filesystem::path &cwd, const std::vector<std::string> &args);
}

#endif //IMPL_EXECUTE_HPP

