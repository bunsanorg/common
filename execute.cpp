#include "execute.hpp"

#include <boost/process.hpp>
#include <boost/filesystem.hpp>

#include "util.hpp"

int bunsan::process::sync_execute(const bunsan::process::context &ctx__)
{
	bunsan::process::context ctx_(ctx__);
	ctx_.build();
	SLOG("trying to execute "<<ctx_.executable()<<" in "<<ctx_.current_path()<<(ctx_.use_path()?" using path":" without using path"));
	boost::process::context ctx;
	if (!ctx_.current_path().empty())
		ctx.work_directory = ctx_.current_path().native();
	ctx.environment = boost::process::self::get_environment();
	ctx.stdout_behavior = boost::process::inherit_stream();
	ctx.stderr_behavior = boost::process::inherit_stream();
	std::string exec_;
	if (ctx_.use_path())
	{
		if (!ctx_.executable().empty() && ++ctx_.executable().begin()==ctx_.executable().end())
			exec_ = boost::process::find_executable_in_path(ctx_.executable().native());
		else
			throw std::runtime_error("You can't try find executable in PATH using empty or non-basename executable name ");
	}
	else
		exec_ = boost::filesystem::absolute(ctx_.executable()).native();
	SLOG("executing \""<<exec_<<"\" in "<<ctx_.current_path()<<" with args");
	for (size_t i = 0; i<ctx_.argv().size(); ++i)
		SLOG("args["<<i<<"] == \""<<ctx_.argv()[i]<<"\"");
	boost::process::child child = boost::process::launch(exec_, ctx_.argv(), ctx);
	boost::process::status status = child.wait();
	if (status.exited())
		return status.exit_status();
	else
		return -1;
}

