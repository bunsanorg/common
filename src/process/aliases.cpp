#include "bunsan/process/execute.hpp"

#include <boost/process.hpp>
#include <boost/filesystem.hpp>

#include "bunsan/util.hpp"

#include "bunsan/process/detail/launch.hpp"
#include "bunsan/process/detail/wait.hpp"

int bunsan::process::detail::wait(const boost::process::child &child, const std::chrono::seconds &timeout)
{
	boost::process::child child_ = child;
	return wait(std::move(child_), timeout);
}

int bunsan::process::detail::wait(const boost::process::child &child)
{
	boost::process::child child_ = child;
	return wait(std::move(child_));
}

boost::process::child bunsan::process::detail::launch(const bunsan::process::context &ctx_)
{
	bunsan::process::context ctx = ctx_;
	return launch(std::move(ctx));
}

int bunsan::process::sync_execute(const bunsan::process::context &ctx_)
{
	bunsan::process::context ctx = ctx_;
	return sync_execute(std::move(ctx));
}

int bunsan::process::sync_execute(bunsan::process::context &&ctx_)
{
	return bunsan::process::detail::wait(bunsan::process::detail::launch(std::move(ctx_)));
}

int bunsan::process::sync_execute(const bunsan::process::context &ctx_, const std::chrono::seconds &timeout)
{
	return bunsan::process::detail::wait(bunsan::process::detail::launch(ctx_), timeout);
}

int bunsan::process::sync_execute(bunsan::process::context &&ctx_, const std::chrono::seconds &timeout)
{
	return bunsan::process::detail::wait(bunsan::process::detail::launch(ctx_), timeout);
}

