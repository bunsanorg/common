#ifndef IMPL_EXECUTE_HPP
#define IMPL_EXECUTE_HPP

#include <string>
#include <vector>

#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>

#include "process.hpp"

namespace bunsan
{
	int sync_execute(const boost::filesystem::path &cwd, const boost::filesystem::path &file, const std::vector<std::string> &args, const boost::optional<boost::filesystem::path> &stdin_file, bool use_path=true);
	process_ptr async_execute(const boost::filesystem::path &cwd, const boost::filesystem::path &file, const std::vector<std::string> &args, const boost::optional<boost::filesystem::path> &stdin_file, bool use_path=true);
	// aliases: exec without mapping stdin
	inline int sync_execute(const boost::filesystem::path &cwd, const boost::filesystem::path &file, const std::vector<std::string> &args, bool use_path=true)
	{
		return sync_execute(cwd, file, args, boost::optional<boost::filesystem::path>(), use_path);
	}
	inline process_ptr async_execute(const boost::filesystem::path &cwd, const boost::filesystem::path &file, const std::vector<std::string> &args, bool use_path=true)
	{
		return async_execute(cwd, file, args, boost::optional<boost::filesystem::path>(), use_path);
	}
	// aliases: exec with args[0]==file
	inline int sync_execute(const boost::filesystem::path &cwd, const std::vector<std::string> &args, const boost::filesystem::path &stdin_file, bool use_path=true)
	{
		return sync_execute(cwd, args.at(0), args, stdin_file, use_path);
	}
	inline process_ptr async_execute(const boost::filesystem::path &cwd, const std::vector<std::string> &args, const boost::filesystem::path &stdin_file, bool use_path=true)
	{
		return async_execute(cwd, args.at(0), args, stdin_file, use_path);
	}
	// aliases: exec with args[0]==file and without mapping stdin
	inline int sync_execute(const boost::filesystem::path &cwd, const std::vector<std::string> &args, bool use_path=true)
	{
		return sync_execute(cwd, args.at(0), args, use_path);
	}
	inline process_ptr async_execute(const boost::filesystem::path &cwd, const std::vector<std::string> &args, bool use_path=true)
	{
		return async_execute(cwd, args.at(0), args, use_path);
	}
}

#endif //IMPL_EXECUTE_HPP

