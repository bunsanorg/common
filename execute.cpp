#include "execute.hpp"

#include <boost/process.hpp>
#include <boost/filesystem.hpp>

#include "util.hpp"

int bunsan::sync_execute(const boost::filesystem::path &cwd, const boost::filesystem::path &executable, const std::vector<std::string> &args, bool use_path)
{
	SLOG("trying to execute "<<executable<<" in "<<cwd<<(use_path?" using path":" without using path"));
	boost::process::context ctx;
	ctx.work_directory = cwd.native();
	ctx.environment = boost::process::self::get_environment();
	ctx.stdout_behavior = boost::process::inherit_stream();
	ctx.stderr_behavior = boost::process::inherit_stream();
	std::string exec_;
	if (use_path)
	{
		if (!executable.empty() && ++executable.begin()==executable.end())
			exec_ = boost::process::find_executable_in_path(executable.native());
		else
			throw std::runtime_error("You can't try find executable in PATH using non-basename executable name ");
	}
	else
		exec_ = boost::filesystem::absolute(executable).native();
	SLOG("executing \""<<exec_<<"\" in "<<cwd<<" with args");
	for (size_t i = 0; i<args.size(); ++i)
		SLOG("args["<<i<<"] == \""<<args[i]<<"\"");
	boost::process::child child = boost::process::launch(exec_, args, ctx);
	boost::process::status status = child.wait();
	if (status.exited())
		return status.exit_status();
	else
		return -1;
}

int bunsan::sync_execute(const boost::filesystem::path &cwd, const std::vector<std::string> &args, bool use_path)
{
	if (args.empty())
		throw std::runtime_error("Nothing to execute!");
	return sync_execute(cwd, args.at(0), args, use_path);
}

