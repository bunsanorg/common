#include "execute.hpp"

#include <boost/process.hpp>
#include <boost/filesystem.hpp>

#include "util.hpp"

int bunsan::process::sync_execute(const bunsan::process::context &ctx__)
{
	bunsan::process::context ctx = ctx__;
	return sync_execute(std::move(ctx));
}

int bunsan::process::sync_execute(bunsan::process::context &&ctx_)
{
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

bunsan::process::context &bunsan::process::context::operator=(context &&ctx)
{
	current_path_ = std::move(ctx.current_path_);
	executable_ = std::move(ctx.executable_);
	argv_ = std::move(ctx.argv_);
	use_path_ = ctx.use_path_;
	return *this;
}

void bunsan::process::context::build()
{
	if (!current_path_)
		current_path_ = boost::filesystem::current_path();
	if (!executable_)
	{
		if (use_path_)
		{
			if (argv_.empty())
				throw std::runtime_error("Nothing to execute!");
			executable_ = argv_.at(0);
		}
		else
			executable_ = boost::filesystem::absolute(argv_.at(0));
	}
}

